/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:54:20 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/13 16:38:48 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_skip_quotes(t_vars *vars);

int	ft_further_join_return(t_shell *shell, char **s, int op)
{
	char	*temp;
	char	*new;
	t_vars	*vars;

	vars = shell->vars;
	new = ft_next_string(shell, op);
	if (shell->vars->malloc_flag == RED && new == NULL)
		return (YES);
	if (vars->expand_it == NO || (vars->expand_it == YES
			&& vars->expanded == YES))
	{
		temp = *s;
		*s = ft_strjoin(*s, new);
		free(temp);
		free(new);
		if (*s == NULL)
			return (YES);
	}
	return (NO);
	// TODO: check the mem leaks for string new
}

static int	ft_cont_parsing(t_shell *shell, char **s, int op)
{
	t_vars	*vars;
	int	ind;
	char	*input_line;

	vars = shell->vars;
	ind = vars->ind;
	input_line = vars->input_line;
	if ((!vars->s_quote && !vars->d_quote) && (input_line[ind] == '\"' || input_line[ind] == '\''))
		ft_skip_quotes(vars);
	// WARN: make sure the stop thing (space after quotes end case) works
	if (vars->stop)
	{
		vars->end = vars->ind;
		vars->qontinue = NO;
	}
	if (ft_further_join_return(shell, s, op) == YES)
		return (ft_free_prompt(shell, YES));
	if (vars->increment == YES)
		vars->end++;
	vars->ind = vars->end;
	return (MALLOC_SUCCESS);
}

int	ft_save_cmd_filename(t_shell *shell, char **s, int op)
{
	char	*input_line;
	int	ind;

	input_line = shell->vars->input_line;
	ind = shell->vars->ind;
	if (input_line[ind] == '\"' || input_line[ind] == '\'')
		ft_skip_quotes(shell->vars);
	*s = ft_next_string(shell, op);
	if (shell->vars->malloc_flag == RED && *s == NULL)
		return (ft_free_prompt(shell, YES));
	if (shell->vars->increment == YES) // WARN: adding 1 here works for all cases?
		shell->vars->end++;
	shell->vars->ind = shell->vars->end;
	while (shell->vars->qontinue == YES)
	{
		if (ft_cont_parsing(shell, s, op) == MALLOC_FAIL)
			return (MALLOC_FAIL);
	}
	return (MALLOC_SUCCESS);
}

int	ft_save_cmd(t_shell *shell)
{
	char	*s;

	// NOTE: PARSE command and its options if there are any
	if (ft_save_cmd_filename(shell, &s, COMMAND) == MALLOC_FAIL)
		return (MALLOC_FAIL);
	if (s != NULL)
	{
		if (!vec_push((*shell->input)->cmd, &s))
		{
			if (s)
				free(s);
			return (ft_free_prompt(shell, YES));
		}
	}
	return (MALLOC_SUCCESS);
}
