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
#include "miniwell.h"
#include <stdio.h>

int ft_token_error(char c, int sgle)
{
    if (sgle)
	c = '\'';
    printf("bash: syntax error near unexpected token  '%c'\n",c); 
    return (1);
}

int ft_special_char(char s)
{
    // NOTE: checks for prohibited special characters and returns 1 if true 
    if (s == '(' || s == ')') 
	return (1);
    if (s == '&' || s == ';' || s == '\\')
	return (1);
    return (0);
}

static int ft_prohibited_chars(t_vars *vars)
{
    // NOTE: loops over the user input and checks for prohibited special characters and returns 1 if true 
    int ind;
    
    ind = -1;
    if (vars->input_line[0] == '|')
	return (1);
    while (++ind < vars->len)
    {
	if (ft_special_char(vars->input_line[ind]))
	    return (ft_token_error(vars->input_line[ind], 0));
    }
    return (0);
}

int ft_space_until_end(char *s)
{
    // NOTE: check if all the characters after in s are spaces 
    // returns 1 if all spaces or 0 when encounters any other character than spaces.
    int ind;
    size_t len;

    ind = -1;
    len = ft_strlen(s);
    while (++ind < len)
    {
	if (!ft_isspace(s[ind])) //WARN: make sure should it be all whitespace chars or just ' ' (single space)
	    return (0);
    }
    return (ind);
}

static void ft_handle_quotes(char *input, int *ind, int *sgle, int *dble)
{
    if (input[*ind] == '\"' && input[*ind+1] != '\"')
    {
	(*dble)++;
	while (input[*ind+1] && input[*ind+1] != '\"')
	    (*ind)++;
	if (input[++(*ind)] == '\"')
	    (*dble)--;
    }
    else if (input[*ind] == '\'' && input[(*ind)+1] != '\'')
    {
	(*sgle)++;
	while (input[(*ind)+1] && input[(*ind)+1] != '\'')
	    (*ind)++;
	if (input[++(*ind)] == '\'')
	    (*sgle)--;
    }
    else
	(*ind)++;
}

static int ft_unclosed_quote(t_vars *vars)
{
    int ind;
    int sgle;
    int dble;

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
    return (0);
}

int ft_syntax_error(t_vars *vars)
{
    // NOTE: writes the errors message and returns 1 when there's error 
    // Errors: 1. if one of the operators come in the beginning or there's an unclosed quotation mark
    //	       2. OR if all the characters are spaces
    //	 TODO: RETURN EXIT STATUS IF THERE's SYNTAX ERROR
    vars->len = ft_strlen(vars->input_line);
    if (ft_space_until_end(vars->input_line))
	return (1);
    if (ft_prohibited_chars(vars))
	return (1);
    if (ft_unclosed_quote(vars))
	return (1);
    return (0);
}
