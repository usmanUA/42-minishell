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
#include "libft/libft.h"
#include "miniwell.h"
#include <sys/_types/_size_t.h>

static int ft_special_char(char *s, size_t len)
{
    size_t  ind;

    ind = 1;
    if (s[0] == '(' || s[0] == ')' || s[0] == '|')
	return (1);
    if (s[0] == '&' || s[0] == ';')
	return (1);
    if (s[0] == '\"' || s[0] == '\'')
    {
	while (ind < len && s[ind] != '|')
	{
	    if (s[0] == '\'' && s[ind] == '\'')
		return (0);
	    else if (s[0] == '\"' && s[ind] == '\"')
		return (0);
	    ind++;
	}
	return (1);
    }
    return (0);
}

int ft_space_until_end(char *s)
{
    int ind;
    size_t len;

    ind = -1;
    len = ft_strlen(s);
    while (++ind < len)
    {
	if (!ft_isspace(s[ind])) //WARN: make sure should it be all whitespace chars or just ' ' (single space)
	    return (0);
    }
    return (1);
}

int ft_syntax_error(t_vars *vars, size_t len)
{
    int ind;

    ind = vars->end;
    if (!len)
    {
	vars->len = ft_strlen(vars->input_line);
	len = vars->len; 
    }
    if (ft_space_until_end(&vars->input_line[ind]))
	return (1);
    if (ft_special_char(&vars->input_line[ind], len))
    {
	printf("bash: syntax error near unexpected token  '%c'\n", vars->input_line[vars->ind]);
	return (1); //TODO: Make it return exit status 258
    }
 //    while (++ind < len)
 //    {
	// if (s[ind] == '|' && ft_special_char(&s[++ind]))
	// {
	//     printf("bash: syntax error near unexpected token  '%c'\n", s[0]);
	//     return (1); //TODO: Make it return exit status 258
	// }
 //    }
    return (0);
}
