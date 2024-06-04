/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:50:16 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/16 13:50:18 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void		ft_skip_spaces(char *s, int *ind);
int	ft_void_redirects(t_vars *vars);

int	ft_space_until_end(t_vars *vars)
{
	int	ind;

	ind = -1;
	while (++ind < vars->len)
	{
		if (!ft_isspace(vars->input_line[ind]))
			return (NO);
	}
	return (YES);
}

static void	ft_handle_quotes(char *input, int *ind, int *sgle, int *dble)
{
	if (input[*ind] == '\"' && input[*ind + 1] != '\"')
	{
		(*dble)++;
		while (input[*ind + 1] && input[*ind + 1] != '\"')
			(*ind)++;
		if (input[++(*ind)] == '\"')
			(*dble)--;
	}
	else if (input[*ind] == '\'' && input[(*ind) + 1] != '\'')
	{
		(*sgle)++;
		while (input[(*ind) + 1] && input[(*ind) + 1] != '\'')
			(*ind)++;
		if (input[++(*ind)] == '\'')
			(*sgle)--;
	}
	else
		(*ind)++;
}

static int	ft_unclosed_quote(t_vars *vars)
{
	int	ind;
	int	sgle;
	int	dble;

	ind = -1;
	sgle = 0;
	dble = 0;
	while (++ind < vars->len)
	{
		if (vars->input_line[ind] == '\"' || vars->input_line[ind] == '\'')
			ft_handle_quotes(vars->input_line, &ind, &sgle, &dble);
	}
	if (sgle || dble)
		return (ft_token_error('\"', sgle));
	return (NO);
}

static	int	ft_void_pipes(t_vars *vars, int ind)
{
	while (++ind < vars->len)
	{
		if (vars->input_line[ind] == '|')
		{
			++ind;
			ft_skip_spaces(vars->input_line, &ind);
			if (ind <= vars->len && vars->input_line[ind] == '\0')
				return (ft_token_error('|', 0));
			if (vars->input_line[ind] == '\"' || vars->input_line[ind] == '\'')
			{
				while (vars->input_line[ind] == '\"'
					|| vars->input_line[ind] == '\'')
				{
					++ind;
					ft_skip_spaces(vars->input_line, &ind);
				}
				ft_skip_spaces(vars->input_line, &ind);
				continue ;
			}
			if (vars->input_line[ind] == '|')
				return (ft_token_error('|', 0));
		}
	}
	return (NO);
}

int	ft_syntax_error(t_vars *vars)
{
	vars->len = ft_strlen(vars->input_line);
	if (ft_void_redirects(vars) == YES)
		return (YES);
	if (ft_space_until_end(vars) == YES)
		return (YES);
	if (ft_unclosed_quote(vars) == YES)
		return (YES);
	if (ft_void_pipes(vars, -1) == YES)
		return (YES);
	if (ft_prohibited_chars(vars) == YES)
		return (YES);
	return (NO);
}
