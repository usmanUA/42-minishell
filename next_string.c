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
#include <stdio.h>

static void  ft_commands_end_index(t_vars *vars, int *ind)
{
    // NOTE: finds the index where the command ends 
    // updates vars->qontinue based on the presence/absence of quotes right next to where command ends
    char c;
    int	qontinue;
    
    qontinue = 0;
    c = vars->input_line[vars->ind + *ind];
    if (c == '$')
	vars->expand_it = 1;
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

static	int ft_valid_char(char next)
{
    if (ft_isdigit(next))
	return (INVALID);
    if (next == '-')
	return (INVALID);
    return (VALID);
}
static void ft_commands_end(t_vars *vars, int quote, int *ind)
{
    // NOTE: knows the command string has started, brings the pointer to the last char of the command string
    // takes care of the quotes if present
    char	c;
    char	next;

    if (vars->d_quote || vars->s_quote)
    {
	if (vars->s_quote)
	    quote = '\'';
	c = vars->input_line[vars->ind + *ind];
	while (c && c != quote)
	{
	    next = vars->input_line[vars->ind + (*ind+1)];
	    if (c == '$' && ft_valid_char(next) == VALID && vars->s_quote == 0)
		vars->expand_it = 1;
	    (*ind)++;
	    c = vars->input_line[vars->ind + *ind];
	}
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

char	*ft_find_value(t_vars *vars, t_envp *env_vars, char *key, int len)
{
    char *value;
    t_envp  *envp;
    int	    not_found;

    value = NULL;
    envp = env_vars;
    not_found = 1;
    while (envp->next)
    {
	if (!ft_strncmp(key, envp->env_name, len))
	{
	    not_found = 0;
	    value = ft_strdup(envp->env_value);	
	    break;
	}
	envp = envp->next;
    }
    if (not_found)
	vars->no_expansion = 1;
    return (value);
}

char	*ft_expand_variable(t_vars *vars, t_envp *env_vars)
{
    int	len;
    char    *str;
    char    *key;
    char    *value;

    len = vars->ind-1;
    if (vars->input_line[vars->ind] == '$')
    {
	key = &vars->input_line[vars->ind+1];
	str = ft_find_value(vars, env_vars, key, vars->end - (vars->ind+1));
    }
    else
    {
	while (++len < vars->end)
	{
	    if (vars->input_line[len] == '$')
		break ;
	}
	str = ft_substr(vars->input_line, vars->ind, len);
	if (!str)
	    return (NULL);
	++len;
	key = &vars->input_line[len];
	value = ft_find_value(vars, env_vars, key, vars->end - len);
	if (value)
	    str = ft_strjoin(str, value);
    }
    return (str);
}

char *ft_next_string(t_vars *vars, int op, t_envp *env_vars)
{
    // NOTE: returns the next string 
    char *s;

    ft_strings_end(vars, op); // NOTE: vars->end now points to the end of string
    if (vars->expand_it)
    {
	vars->expand_it = 0;
	s = ft_expand_variable(vars, env_vars);
    }
    else
	s = ft_substr(vars->input_line, vars->ind, vars->end - vars->ind); // NOTE: malloc that string in heap and point str to it
    if (!s)
	return (NULL); // WARN: malloc fail
    return (s);
}

