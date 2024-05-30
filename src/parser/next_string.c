/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next_string.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 09:40:12 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/30 03:12:46 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_status_expansion(t_vars *vars, char c, int *ind);
char		*ft_expand_variable(t_shell *shell, int op);
int			ft_valid_char(char next, int check_digits);
void		ft_quoted_str_end(t_vars *vars, int quote, int *ind, int *dollar);
void		ft_unquoted_str_end(t_vars *vars, int *ind);

static void	ft_verify_expansions(t_vars *vars, int *ind)
{
	char	next;

	++(*ind);
	if ((vars->ind + *ind) >= vars->len)
		return ;
	next = vars->input_line[vars->ind + *ind];
	if (ft_valid_char(next, YES) == VALID && !vars->s_quote)
		vars->expand_it = YES;
	else if ((next == '\"' || next == '\'') && (vars->d_quote || vars->s_quote))
	{
		vars->expand_it = NO;
		if ((next == '\'' && vars->s_quote) || (next == '\"' && vars->d_quote))
		{
			vars->qontinue = YES;
			vars->s_quote = 0;
			vars->d_quote = 0;
			vars->increment = YES;
		}
	}
}

static void	ft_commands_end(t_vars *vars, int quote, int *ind)
{
	char	c;
	char	next;
	int		dollar;

	dollar = NO;
	c = vars->input_line[vars->ind];
	if (c == '$' && vars->heredoc == NO)
		ft_verify_expansions(vars, ind);
	if ((vars->ind + *ind) >= vars->len)
		return ;
	c = vars->input_line[vars->ind + *ind];
	if (ft_status_expansion(vars, c, ind) == YES)
		return ;
	if (vars->increment == YES)
		return ;
	if (vars->d_quote || vars->s_quote)
	{
		if (vars->s_quote)
			quote = '\'';
		ft_quoted_str_end(vars, quote, ind, &dollar);
		return ;
	}
	ft_unquoted_str_end(vars, ind);
}

static void	ft_strings_end(t_vars *vars, int operator)
{
	int	ind;

	ind = 0;
	if (operator == REDIRECT)
	{
		while (vars->input_line[vars->ind + ind] == '<'
			|| vars->input_line[vars->ind + ind] == '>')
		{
			ind++;
			if (ind == 2)
				break ;
		}
	}
	else if (operator == FD)
	{
		while (ft_isdigit(vars->input_line[vars->ind + ind]))
			ind++;
	}
	else if (operator == COMMAND || operator == FILENAME)
		ft_commands_end(vars, '\"', &ind);
	vars->end = vars->ind + ind;
}

char	*ft_next_string(t_shell *shell, int op)
{
	char	*s;
	t_vars	*vars;

	vars = shell->vars;
	vars->expand_it = NO;
	vars->expanded = NO;
	vars->qontinue = NO;
	vars->increment = NO;
	vars->malloc_flag = GREEN;
	ft_strings_end(vars, op);
	if (vars->expand_it == YES)
		s = ft_expand_variable(shell, op);
	else
		s = ft_substr(vars->input_line, vars->ind, vars->end - vars->ind);
	if ((vars->expand_it == NO && !s) || (vars->expand_it == YES
			&& vars->expanded == YES && !s))
		vars->malloc_flag = RED;
	return (s);
}
