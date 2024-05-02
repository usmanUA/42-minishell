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
static int ft_special_char(char s, int pipe_flag, int redir_flag)
{
    // NOTE: checks for prohibited special characters and returns 1 if true 
    if (pipe_flag != NOFLAG && redir_flag != NOFLAG)
    {
	if (s == '|')
	    return (1);
	if (s == '<' || s == '>')
	    return (1);
    }
    if (s == '(' || s == ')') 
	return (1);
    if (s == '&' || s == ';' || s == '\\')
	return (1);
    return (0);
}

static int ft_redirect_operator(char *s, int *ind)
{
    if (!ft_strncmp(&s[*ind], "<<", 2) || !ft_strncmp(&s[*ind], ">>", 2))
    {
	*ind += 2;
	return (1);
    }
    else if (!ft_strncmp(&s[*ind], "<", 1) || !ft_strncmp(&s[*ind], ">", 1))
    {
	++(*ind);
	return (1);
    }
    return (0);
}

static void ft_skip_spaces(char *s, int *ind)
{
    while (s[*ind] != '\0' && ft_isspace(s[*ind]))
	    ++(*ind);
}

static int ft_prohibited_chars(t_vars *vars)
{
    // NOTE: loops over the user input and checks for prohibited special characters and returns 1 if true 
    int ind;
    
    ind = 0;
    ft_skip_spaces(vars->input_line, &ind);
    if (vars->input_line[ind] == '|')
	return (1);
    while (ind < vars->len)
    {
	if (ft_redirect_operator(vars->input_line, &ind))
	{
	    ft_skip_spaces(vars->input_line, &ind);
	    if (ft_special_char(vars->input_line[ind], PIPE, REDIR))
		return (ft_token_error(vars->input_line[ind], 0));
	}
	else if (ft_special_char(vars->input_line[ind], NOFLAG, NOFLAG))
	    return (ft_token_error(vars->input_line[ind], 0));
	++ind;
    }
    return (0);
}

int ft_space_until_end(t_vars *vars)
{
    // NOTE: check if all the characters after in s are spaces 
    // returns 1 if all spaces or 0 when encounters any other character than spaces.
    int ind;

    ind = -1;
    while (++ind < vars->len)
    {
	if (!ft_isspace(vars->input_line[ind])) //WARN: make sure should it be all whitespace chars or just ' ' (single space)
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
    if (ft_space_until_end(vars))
	return (1);
    if (ft_prohibited_chars(vars))
	return (1);
    if (ft_unclosed_quote(vars))
	return (1);
    return (0);
}
