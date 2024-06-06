/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 17:32:13 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/07 17:32:24 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_special_char(char s, int check_pipe_redirs)
{
	if (check_pipe_redirs == YES)
	{
		if (s == '|')
			return (YES);
		if (s == '<' || s == '>')
			return (YES);
	}
	if (s == '(' || s == ')')
		return (YES);
	if (s == '&' || s == ';' || s == '\\')
		return (YES);
	return (NO);
}

static int	ft_redirect_operator(char *s, int *ind)
{
	if (!ft_strncmp(&s[*ind], "<<", 2) || !ft_strncmp(&s[*ind], ">>", 2))
	{
		*ind += 2;
		return (YES);
	}
	else if (!ft_strncmp(&s[*ind], "<", 1) || !ft_strncmp(&s[*ind], ">", 1))
	{
		++(*ind);
		return (YES);
	}
	return (NO);
}

void	ft_skip_spaces(char *s, int *ind)
{
	while (s[*ind] != '\0' && ft_isspace(s[*ind]))
		++(*ind);
}

void	ft_skip_enclosed(t_vars *vars, int *ind)
{
	char	c;

	c = vars->input_line[*ind];
	if (c != '\'' && c != '\"')
		return ;
	++(*ind);
	while (vars->input_line[*ind] && vars->input_line[*ind] != c)
		++(*ind);
	if (*ind < vars->len)
		++(*ind);
}

int	ft_prohibited_chars(t_vars *vars)
{
	int	ind;

	ind = 0;
	ft_skip_spaces(vars->input_line, &ind);
	if (vars->input_line[ind] == '|')
		return (ft_token_error(vars->input_line[ind], 0));
	while (ind < vars->len)
	{
		ft_skip_enclosed(vars, &ind);
		if (ft_redirect_operator(vars->input_line, &ind) == YES)
		{
			ft_skip_spaces(vars->input_line, &ind);
			if (ft_special_char(vars->input_line[ind], YES) == YES)
				return (ft_token_error(vars->input_line[ind], 0));
		}
		else if (ft_special_char(vars->input_line[ind], NO) == YES)
			return (ft_token_error(vars->input_line[ind], 0));
		++ind;
	}
	return (NO);
}
