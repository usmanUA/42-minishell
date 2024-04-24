/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next_string.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 09:40:12 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/24 09:40:14 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "miniwell.h"

static int  ft_not_space_nor_quote(char c, int quote)
{
    if (quote == 0)
    {
	if (c == '\0' || ft_isspace(c))
	    return (0);
	return (1);
    }
    if (c == '<' || c == '>')
	return (0);
    if (c != '\0' && !ft_isspace(c) && c != '\'' && c != '\"')// WARN: LOOK BACK
	return (1);
    return (0);
}

static void ft_commands_end(t_vars *vars, int quote, int *ind)
{
    // NOTE: knows the command string has started, brings the pointer to the last char of the command string
    // takes care of the quotes if present
    if (vars->d_quote || vars->s_quote)
    {
	if (vars->s_quote)
	    quote = '\'';
	while (vars->input_line[vars->ind + *ind] && vars->input_line[vars->ind + *ind] != quote)
	    (*ind)++;
	if (!ft_isspace(vars->input_line[vars->ind + (*ind+1)]))
	    vars->qontinue = 1;
	vars->s_quote = 0;
	vars->d_quote = 0;
	vars->increment = 1;
	return ;
    }
    while (ft_not_space_nor_quote(vars->input_line[vars->ind + *ind], quote))
	(*ind)++;
    if (!ft_not_space_nor_quote(vars->input_line[vars->ind + *ind], 0))
	vars->qontinue = 0;

}
static void	ft_strings_end(t_vars *vars, int operator)
{
    // NOTE: finds the index for the end of the current string and update the var vars->end 
    // if there's operator then finds the index of next character followed by the operator
    int ind;

    ind = 0;
    if (operator == REDIRECT)
    {
	while (vars->input_line[vars->ind+ind]=='<' || vars->input_line[vars->ind+ind]=='>')
	    ind++;
    }
    else if (operator == FD)
    {
	while (ft_isdigit(vars->input_line[vars->ind+ind]))
	    ind++;
    }
    else if (operator == COMMAND)
	ft_commands_end(vars, '\"', &ind);
    vars->end = vars->ind + ind; // NOTE: END updates here
}

char *ft_next_string(t_vars *vars, int op)
{
    // NOTE: returns the next string 
    char *s;

    if (!ft_index_after_spaces(vars)) // NOTE: skip spaces, vars->ind->string or 0 if all spaces 
	 return (NULL);
    ft_strings_end(vars, op); // NOTE: vars->end now points to the end of string
    s = ft_substr(vars->input_line, vars->ind, vars->end - vars->ind); // NOTE: malloc that string in heap and point str to it
    if (!s)
	return (NULL);
    return (s);
}

