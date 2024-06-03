/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next_string_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:34:48 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/07 16:34:50 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_valid_char(char next, int check_digits)
{
	if (next == '\0')
		return (INVALID);
	if (check_digits == YES)
	{
		if (ft_isdigit(next))
			return (INVALID);
	}
	if (next == '-')
		return (INVALID);
	if (next == '\"' || next == '\'')
		return (INVALID);
	if (ft_isspace(next))
		return (INVALID);
	return (VALID);
}

int	ft_status_expansion(t_vars *vars, char c, int *ind)
{
	char	next;

	next = '\0';
	if (c == '?')
	{
		++(*ind);
		if ((vars->ind + *ind) < vars->len)
		{
			next = vars->input_line[vars->ind + 1];
			if (!ft_isspace(next))
				vars->qontinue = YES;
		}
		return (YES);
	}
	return (NO);
}

int	expandable_char(int expand_it, char c)
{
	if (expand_it == YES)
	{
		if (c == ':' || c == '-' || c == ',')
			return (NO);
		if (c == '.' || c == '+' || c == '=' || c == '~')
			return (NO);
		if (c == '!' || c == '/' || c == '@' || c == '?')
			return (NO);
		if (c == '#' || c == '^' || c == '{' || c == '}')
			return (NO);
		if (c == '[' || c == ']' || c == '%')
			return (NO);
	}
	return (YES);
}
