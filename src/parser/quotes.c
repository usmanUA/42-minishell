/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:52:39 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/22 13:34:24 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_valid_char(char next, int check_digits);

static int	ft_quote_skipped(t_vars *vars, char quo)
{
	int	len;

	len = vars->len;
	if (vars->input_line[++vars->ind] != quo)
		return (NO);
	++vars->ind;
	if (ft_isspace(vars->input_line[vars->ind]) || vars->ind == len)
		vars->stop = YES;
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

void	ft_unquoted_str_end(t_vars *vars, int *ind)
{
	char	c;

	c = vars->input_line[vars->ind + *ind];
	while (c != '\0' && c != '|')
	{
		if (vars->heredoc == NO && c == '$')
			break ;
		if (c == '\'' || c == '\"')
		{
			vars->qontinue = YES;
			break ;
		}
		else if (ft_isspace(c) || c == '<' || c == '>')
			break ;
		c = vars->input_line[vars->ind + ++(*ind)];
	}
	if (c == '$')
		vars->qontinue = YES;
}

static	void	ft_set_flags(t_vars *vars, char next, int *dollar)
{
	if (*dollar == YES)
		return ;
	if (!ft_isspace(next) && next != '\0')
		vars->qontinue = YES;
	vars->s_quote = 0;
	vars->d_quote = 0;
	vars->increment = YES;
}

void	ft_quoted_str_end(t_vars *vars, int quote, int *ind, int *dollar)
{
	char	c;
	char	next;

	c = vars->input_line[vars->ind + *ind];
	next = vars->input_line[vars->ind + *ind + 1];
	while (c && c != quote)
	{
		if (vars->heredoc == NO && c == '$' && ft_valid_char(next, YES) == VALID
			&& vars->s_quote == 0)
		{
			vars->qontinue = YES;
			*dollar = YES;
			break ;
		}
		else if (vars->expand_it == YES && c != '\"' && c != '_'
			&& !ft_isalnum(c))
		{
			vars->qontinue = YES;
			return ;
		}
		(*ind)++;
		c = next;
		next = vars->input_line[vars->ind + (*ind + 1)];
	}
	ft_set_flags(vars, next, dollar);
}
