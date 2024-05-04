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

    // TODO: check the cases -> 0;, 0| (cases where fd is followed by special chars other than redirects)
int ft_space_until_end(t_vars *vars)
{
    // NOTE: check if all the characters after in s are spaces 
    // returns 1 if all spaces or 0 when encounters any other character than spaces.
    int ind;

    ind = -1;
    while (++ind < vars->len)
    {
	if (!ft_isspace(vars->input_line[ind])) //WARN: make sure should it be all whitespace chars or just ' ' (single space)
	    return (NO);
    }
    return (YES);
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
    return (NO);
}

int ft_syntax_error(t_vars *vars)
{
    // NOTE: writes the errors message and returns 1 when there's error 
    // Errors: 1. if one of the operators come in the beginning or there's an unclosed quotation mark
    //	       2. OR if all the characters are spaces
    //	 TODO: RETURN EXIT STATUS IF THERE's SYNTAX ERROR
    char    c;
    char    next;
    char    null_term;

    null_term = '\0';
    vars->len = ft_strlen(vars->input_line);
    c = vars->input_line[0];
    if (vars->len > 1)
    {
	next = vars->input_line[1];
	if ((c == '\'' && next == '\'') || (c == '\"' && next == '\"'))
	{
	    ft_cmd_error(&null_term, 1, 1);
	    return (YES);
	}
    }
    if (ft_space_until_end(vars) == YES)
	return (YES);
    if (ft_prohibited_chars(vars) == YES)
	return (YES);
    if (ft_unclosed_quote(vars) == YES)
	return (YES);
    return (NO);
}
