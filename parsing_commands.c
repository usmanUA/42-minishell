/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   after_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:54:20 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/18 14:54:22 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_quote_skipped(t_vars *vars, char quo)
{
	if (vars->input_line[++vars->ind] != quo)
		return (NO);
	++vars->ind;
	if (ft_isspace(vars->input_line[vars->ind]
			|| vars->input_line[vars->ind] == '\0'))
		vars->stop = 1;
	return (YES);
}

void	ft_skip_quotes(t_vars *vars)
{
	while (42)
	{
		if (vars->input_line[vars->ind] == '\"')
		{
			if (ft_quote_skipped(vars, '\"') == NO)
			{
				vars->d_quote = 1;
				return ;
			}
		}
		else if (vars->input_line[vars->ind] == '\'')
		{
			if (ft_quote_skipped(vars, '\'') == NO)
			{
				vars->s_quote = 1;
				return ;
			}
		}
		else
			break ;
	}
}

static int	ft_cont_parsing(t_shell *shell, char **s, int op)
{
	char	*temp;
	char	*new;
	t_vars	*vars;

	temp = NULL;
	vars = shell->vars;
	if ((!vars->s_quote && !vars->d_quote) && (vars->input_line[vars->ind] == '\"' || vars->input_line[vars->ind] == '\''))
		ft_skip_quotes(vars);
	// WARN: make sure the stop thing (space after quotes end case) works
	if (vars->stop)
	{
		vars->end = vars->ind;
		vars->qontinue = NO;
	}
	new = ft_next_string(shell, op);
	if (shell->vars->malloc_flag == RED && new == NULL)
		return (ft_free_prompt(shell, YES));
	if (vars->expand_it == NO || (vars->expand_it == YES
			&& vars->expanded == YES))
	{
		temp = *s;
		*s = ft_strjoin(*s, new);
		free(temp);
		free(new);
		if (*s == NULL)
			return (ft_free_prompt(shell, YES));
	}
	// TODO: check the mem leaks for string new
	if (vars->increment == YES)
		vars->end++;
	vars->ind = vars->end;
	return (MALLOC_SUCCESS);
}

int	ft_save_cmd_filename(t_shell *shell, char **s, int op)
{
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
	char	*input_line;
	int	ind;

	// NOTE: PARSE command and its options if there are any
	input_line = shell->vars->input_line;
	ind = shell->vars->ind;
	if (input_line[ind] == '\"' || input_line[ind] == '\'')
		ft_skip_quotes(shell->vars);
	if (ft_save_cmd_filename(shell, &s, COMMAND) == MALLOC_FAIL)
		return (MALLOC_FAIL);
	if (!vec_push((*shell->input)->cmd, &s))
	{
		if (s)
			free(s);
		return (ft_free_prompt(shell, YES));
	}
	return (MALLOC_SUCCESS);
}
