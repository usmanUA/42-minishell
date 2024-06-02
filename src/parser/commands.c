/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:54:20 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/22 13:35:43 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_skip_quotes(t_vars *vars);

void	ft_skip_quotes_dollars(t_shell *shell, int inside_quotes)
{
	char	*input_line;
	int		ind;
	t_vars	*vars;

	input_line = shell->vars->input_line;
	ind = shell->vars->ind;
	vars = shell->vars;
	if (inside_quotes == YES)
	{
		if ((!vars->s_quote && !vars->d_quote) && (input_line[ind] == '\"'
				|| input_line[ind] == '\''))
			ft_skip_quotes(vars);
	}
	else if (inside_quotes == NO)
	{
		if (input_line[ind] == '\"' || input_line[ind] == '\'')
			ft_skip_quotes(shell->vars);
	}
	ind = shell->vars->ind;
	while (input_line[ind] == '$' && input_line[ind + 1] == '$')
		ind += 2;
	shell->vars->ind = ind;
}

int	ft_further_join_return(t_shell *shell, char **s, int op)
{
	char	*temp;
	char	*new;
	t_vars	*vars;

	vars = shell->vars;
	new = ft_next_string(shell, op);
	if (shell->vars->malloc_flag == RED && new == NULL)
		return (YES);
	if (*s == NULL && new != NULL)
	{
		*s = ft_strdup(new);
		free(new);
	}
	else if (vars->expand_it == NO || (vars->expand_it == YES
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
}

static int	ft_cont_parsing(t_shell *shell, char **s, int op)
{
	t_vars	*vars;

	vars = shell->vars;
	ft_skip_quotes_dollars(shell, YES);
	if (vars->stop == YES)
	{
		vars->stop = NO;
		vars->end = vars->ind;
		vars->qontinue = NO;
		return (SUCCESS);
	}
	if (ft_further_join_return(shell, s, op) == YES)
		return (ft_free_prompt(shell, YES));
	if (vars->increment == YES)
		vars->end++;
	vars->ind = vars->end;
	return (SUCCESS);
}

int	ft_save_cmd_filename(t_shell *shell, char **s, int op)
{
	ft_skip_quotes_dollars(shell, NO);
	if (shell->vars->stop == YES)
	{
		shell->vars->stop = NO;
		shell->vars->end = shell->vars->ind;
		return (SUCCESS);
	}
	*s = ft_next_string(shell, op);
	if (shell->vars->malloc_flag == RED && *s == NULL)
		return (ft_free_prompt(shell, YES));
	if (shell->vars->increment == YES)
		shell->vars->end++;
	shell->vars->ind = shell->vars->end;
	while (shell->vars->qontinue == YES)
	{
		if (ft_cont_parsing(shell, s, op) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	ft_save_cmd(t_shell *shell)
{
	char	*s;

	s = NULL;
	if (ft_save_cmd_filename(shell, &s, COMMAND) == FAILURE)
		return (FAILURE);
	if (s != NULL)
	{
		if (!vec_push((*shell->input)->cmd, &s))
		{
			if (s)
				free(s);
			return (ft_free_prompt(shell, YES));
		}
	}
	return (SUCCESS);
}
