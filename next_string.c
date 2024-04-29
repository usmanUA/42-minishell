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

static void  ft_commands_end_index(t_vars *vars, int *ind)
{
    // NOTE: finds the index where the command ends 
    // updates vars->qontinue based on the presence/absence of quotes right next to where command ends
    char c;
    int	qontinue;
    
    qontinue = 0;
    c = vars->input_line[vars->ind + *ind];
    while (c != '\0')
    {
	if (c == '\'' || c == '\"')
	{
	    qontinue = 1;
	    break ;
	}
	else if (ft_isspace(c) || c == '<' || c == '>') // NOTE: do we need to check for redirect operators?
	    break ;
	(*ind)++;
	c = vars->input_line[vars->ind + *ind];
    }
    vars->qontinue = qontinue;
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
    ft_commands_end_index(vars, ind);
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
	{
	    ind++;
	    if (ind == 2)
		break ;
	}
    }
    else if (operator == FD)
    {
	while (ft_isdigit(vars->input_line[vars->ind+ind]))
	    ind++;
    }
    else if (operator == COMMAND || operator == FILENAME)
	ft_commands_end(vars, '\"', &ind);
    vars->end = vars->ind + ind; // NOTE: END updates here
}

char *ft_next_string(t_vars *vars, int op)
{
    // NOTE: returns the next string 
    char *s;

    ft_strings_end(vars, op); // NOTE: vars->end now points to the end of string
    s = ft_substr(vars->input_line, vars->ind, vars->end - vars->ind); // NOTE: malloc that string in heap and point str to it
    if (!s)
	return (NULL); // WARN: malloc fail

    return (s);
}

