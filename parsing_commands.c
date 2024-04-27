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
#include "vec/vec.h"

int	ft_not_skipped(t_vars *vars, char quo)
{
    if (vars->input_line[++vars->ind] != quo)
	return (1);
    ++vars->ind;
    if (ft_isspace(vars->input_line[vars->ind] || vars->input_line[vars->ind] == '\0'))
	vars->stop = 1;
    return (0);
}

void	ft_skip_quotes(t_vars *vars)
{
    while (42)
    {
	if (vars->input_line[vars->ind] == '\"')
	{
	    if (ft_not_skipped(vars, '\"'))
	    {
		vars->d_quote = 1;
		return ;
	    }
	}
	else if (vars->input_line[vars->ind] == '\'')
	{
	    if (ft_not_skipped(vars, '\''))
	    {
		vars->s_quote = 1;
		return ;
	    }
	}
	else
	    break ;
    }
}

int ft_save_cmd_filename(t_vars *vars, char **s)
{
    char *temp;

    *s = ft_next_string(vars, COMMAND);
    if (!*s)
	return (0); // NOTE: Either malloc fail or all spaces until '\0'
    if (vars->increment)// WARN: adding 1 here works for all cases?
	vars->end++;
    vars->ind = vars->end;
    vars->increment = 0;
    while (vars->qontinue)
    {
// PERF:     ""'"""""""""""'cat"''''''''  "'' main.c '\0'
	temp = *s;
	if (vars->input_line[vars->ind] == '\"' || vars->input_line[vars->ind] == '\'')
	    ft_skip_quotes(vars);
	// WARN: make sure the stop thing (space after quotes end case) works 
	if (vars->stop)
	{
	    vars->end = vars->ind;
	    break ;
	}
	*s = ft_strjoin(*s, ft_next_string(vars, COMMAND));
	if (!*s)
	{
	    free(temp);
	    return (0);
	}
	free(temp);
	if (vars->increment)
	    vars->end++; 
	vars->ind = vars->end;
	vars->increment = 0;
    }
    return (1);
}

int ft_save_cmd(t_vec *cmd, t_vars *vars)
{
    // NOTE: PARSE command and its options if there are any
    char *temp;
    char *s;

    if (vars->input_line[vars->ind] == '\"' || vars->input_line[vars->ind] == '\'')
	ft_skip_quotes(vars);
    if (!ft_save_cmd_filename(vars, &s))
	return (0);
    if (!vec_push(cmd, &s))
    {
	free(s);
	return (0);
    }
    return (1);
}

