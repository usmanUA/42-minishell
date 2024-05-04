/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   after_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:54:20 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/18 14:54:22 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "miniwell.h"

int	ft_quote_skipped(t_vars *vars, char quo)
{
    if (vars->input_line[++vars->ind] != quo)
	return (NO);
    ++vars->ind;
    if (ft_isspace(vars->input_line[vars->ind] || vars->input_line[vars->ind] == '\0'))
	vars->stop = 1;
    return (YES);
}

void	ft_skip_quotes(t_vars *vars)
{
    while (42)
    {
	if (vars->input_line[vars->ind] == '\"')
	{
	    if (ft_quote_skipped(vars, '\"') == NO)
	    {
		vars->d_quote = 1;
		return ;
	    }
	}
	else if (vars->input_line[vars->ind] == '\'')
	{
	    if (ft_quote_skipped(vars, '\'') == NO)
	    {
		vars->s_quote = 1;
		return ;
	    }
	}
	else
	    break ;
    }
}

static	int ft_cont_parsing(t_vars *vars, char **s, t_envp *env_vars, int op)
{
    char *temp;
    char *new;

    temp = NULL;
    if (vars->input_line[vars->ind] == '\"' || vars->input_line[vars->ind] == '\'')
	ft_skip_quotes(vars);
    // WARN: make sure the stop thing (space after quotes end case) works 
    if (vars->stop)
    {
	vars->end = vars->ind;
	vars->qontinue = NO;
    }
    new = ft_next_string(vars, op, env_vars);
    if (vars->expand_it == NO || (vars->expand_it == YES && vars->expanded == YES))
    {
	temp = *s;
	*s = ft_strjoin(*s, new);
	free(temp);
	if (!*s)
	    return (0);
    }
    if (vars->increment == YES)
	vars->end++; 
    vars->ind = vars->end;
    return (1);
}

int ft_save_cmd_filename(t_vars *vars, char **s, t_envp *env_vars, int op)
{
    *s = ft_next_string(vars, op, env_vars);
    if (vars->malloc_flag == RED && !*s)
    {
	return (0); // NOTE: Either malloc fail or all spaces until '\0'
    }
    if (vars->increment == YES)// WARN: adding 1 here works for all cases?
	vars->end++;
    vars->ind = vars->end;
    while (vars->qontinue == YES)
    {
	if (!ft_cont_parsing(vars, s, env_vars, op))
	    return (0);
    }
    return (1);
}

int ft_save_cmd(t_vec *cmd, t_vars *vars, t_envp *env_vars)
{
    // NOTE: PARSE command and its options if there are any
    char *temp;
    char *s;

    if (vars->input_line[vars->ind] == '\"' || vars->input_line[vars->ind] == '\'')
	ft_skip_quotes(vars);
    if (!ft_save_cmd_filename(vars, &s, env_vars, COMMAND))
	return (0);
    if (s)
    {
	if (!vec_push(cmd, &s))
	{
	    if (s)
		free(s);
	    return (0);
	}
    }
    return (1);
}

