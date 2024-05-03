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
#include "libft/libft.h"
#include "miniwell.h"
#include <stdio.h>

static	int ft_valid_char(char next)
{
    if (ft_isdigit(next))
	return (INVALID);
    if (next == '-')
	return (INVALID);
    if (ft_isspace(next))
	return (INVALID);
    return (VALID);
}

static void  ft_commands_end_index(t_vars *vars, int *ind, char c)
{
    // NOTE: finds the index where the command ends 
    // updates vars->qontinue based on the presence/absence of quotes right next to where command ends
    char next;
    
    while (c != '\0' && c != '$')
    {
	if (c == '\'' || c == '\"')
	{
	    vars->qontinue = YES;
	    break ;
	}
	else if (ft_isspace(c) || c == '<' || c == '>') // NOTE: do we need to check for redirect operators?
	    break ;
	c = vars->input_line[vars->ind + ++(*ind)];
    }
    next = vars->input_line[vars->ind + (*ind+1)];
    if (c == '$' && ft_valid_char(next) == VALID)
	vars->qontinue = YES;
}

static void ft_commands_end(t_vars *vars, int quote, int *ind)
{
    // NOTE: knows the command string has started, brings the pointer to the last char of the command string
    // takes care of the quotes if present
    char	c;
    char	next;
    int		dollar;

    dollar = NO;
    c = vars->input_line[vars->ind + *ind];
    next = vars->input_line[vars->ind + (*ind+1)];
    if (c == '$' && ft_valid_char(next) == VALID)
    {
	vars->expand_it = YES;
	c = next;
    }
    if (vars->d_quote || vars->s_quote)
    {
	if (vars->s_quote)
	    quote = '\'';
	while (c && c != quote)
	{
	    if (c == '$' && ft_valid_char(next) == VALID && vars->s_quote == 0)
	    {
		vars->qontinue = YES;
		dollar = YES;
		break;
	    }
	    (*ind)++;
	    c = next;
	    next = vars->input_line[vars->ind + (*ind+1)];
	}
	if (dollar == NO)
	{
	    if (!ft_isspace(vars->input_line[vars->ind + (*ind+1)]))
		vars->qontinue = YES;
	    vars->s_quote = 0;
	    vars->d_quote = 0;
	    vars->increment = YES;
	}
	return ;
    }
    ft_commands_end_index(vars, ind, c);
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

    value = NULL;
    envp = env_vars;
    while (envp->next)
    {
	if (!ft_strncmp(key, envp->env_name, len))
	{
	    vars->expanded = YES;
	    value = ft_strdup(envp->env_value);	
	    break;
	}
	envp = envp->next;
    }
    return (value);
}

char	*ft_expand_variable(t_vars *vars, t_envp *env_vars, int op)
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
	if (op == FILENAME && vars->expand_it == YES && vars->expanded == NO)
	{
	    key = ft_substr(vars->input_line, vars->ind, vars->end - vars->ind);
	    if (!key)
		return (NULL);
	    ft_filerror(0, key, YES);
	    free(key);
	}
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

    vars->expand_it = NO;
    vars->expanded = NO;
    vars->qontinue = NO;
    vars->increment = NO;
    ft_strings_end(vars, op); // NOTE: vars->end now points to the end of string
//    printf("here: %s\n", &vars->input_line[vars->ind]);
    if (vars->expand_it == YES)
	s = ft_expand_variable(vars, env_vars, op);
    else
	s = ft_substr(vars->input_line, vars->ind, vars->end - vars->ind); // NOTE: malloc that string in heap and point str to it
    if ((vars->expand_it == NO && !s) || (vars->expanded == YES && vars->expanded == YES && !s)) // TODO: look back to the condition
	vars->malloc_flag = RED;
    return (s);
}

