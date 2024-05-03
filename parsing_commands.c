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
#include "libft/libft.h"
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

int ft_save_cmd_filename(t_vars *vars, char **s, t_envp *env_vars)
{
    char *temp;

    *s = ft_next_string(vars, COMMAND, env_vars);
    if (vars->no_expansion == 0 && !*s)
    {
	return (0); // NOTE: Either malloc fail or all spaces until '\0'
    }
    if (vars->increment)// WARN: adding 1 here works for all cases?
	vars->end++;
    vars->ind = vars->end;
    vars->increment = 0;
    while (vars->qontinue)
    {
	temp = *s;
	if (vars->input_line[vars->ind] == '\"' || vars->input_line[vars->ind] == '\'')
	    ft_skip_quotes(vars);
	// WARN: make sure the stop thing (space after quotes end case) works 
	if (vars->stop)
	{
	    vars->end = vars->ind;
	    break ;
	}
	*s = ft_strjoin(*s, ft_next_string(vars, COMMAND, env_vars)); // WARN: Only malloc fail?
	if (!*s)
	{
	    if (temp)
		free(temp);
	    return (0);
	}
	if (temp)
	    free(temp);
	if (vars->increment)
	    vars->end++; 
	vars->ind = vars->end;
	vars->increment = 0;
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
    if (!ft_save_cmd_filename(vars, &s, env_vars))
	return (0);
    if (!vec_push(cmd, &s))
    {
	if (s)
	    free(s);
	return (0);
    }
    return (1);
}

