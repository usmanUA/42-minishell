/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:40:04 by uahmed            #+#    #+#             */
/*   Updated: 2024/06/04 15:40:05 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

void		ft_skip_spaces(char *s, int *ind);

int	ft_newline_token_error(t_vars *vars, int ind)
{
	char	next;
	char	last;

	next = vars->input_line[ind];
	if (next == '<' || next == '>')
		++ind;
	ft_skip_spaces(vars->input_line, &ind);
	last = vars->input_line[ind];
	if (last != '\0')
		return (NO);
	printf("miniwell: syntax error near unexpected token  `newline'\n");
	return (YES);
}

int	ft_void_redirects(t_vars *vars)
{
	int	ind;
	char	c;
	int	flag;

	ind = -1;
	flag = NO;
	while (++ind < vars->len)
	{
		c = vars->input_line[ind];
		if (c == '<')
			flag = ft_newline_token_error(vars, ++ind);
		else if (c == '>')
			flag = ft_newline_token_error(vars, ++ind);
		if (flag == YES)
			return (flag);
	}
	return (flag);
}
