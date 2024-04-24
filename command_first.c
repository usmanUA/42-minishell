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
#include <stdlib.h>

int	ft_not_skipped(t_vars *vars, char quo)
{
    if (vars->input_line[++vars->ind] != quo)
	return (1);
    ++vars->ind;
    if (ft_isspace(vars->input_line[vars->ind]))
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

int ft_save_cmd(t_input *input, char *s, t_vars *vars)
{
    // NOTE: PARSE command and its options if there are any
    t_vec cmd;
    char *temp;

    if (!vec_new(&cmd, 2, sizeof(char *))) // NOTE: Initialize a vec and allocate some mem for command
	return (0);
    if (vars->input_line[vars->ind] == '\"' || vars->input_line[vars->ind] == '\'')
	ft_skip_quotes(vars);
    s = ft_next_string(vars, 0);
    if (!s)
	return (0);
    vars->ind = vars->end+1; // WARN: adding 1 here works for all cases?
    while (vars->qontinue)
    {
// PERF: ""'"""""""""""'cat"''''''''  "'' main.c
	temp = s;
	if (vars->input_line[vars->ind] == '\"' || vars->input_line[vars->ind] == '\'')
	    ft_skip_quotes(vars);
	// WARN: make sure the stop thing (space after quotes end case) works 
	if (vars->stop)
	    break ;
	s = ft_strjoin(s, ft_next_string(vars, COMMAND)); // TODO: check for malloc fail
	free(temp); // NOTE: s is coming as an arg, so the first free call might not work
	vars->ind = vars->end;
	if (vars->increment)
	    vars->ind++; 
	vars->increment = 0;
    }
    if (!vec_push(&cmd, s))
	return (0);
    input->cmd = &cmd;
    return (1);
}

// NOTE: in the while loop above:
// we got 13", then cat and then 8'2spcs which makes s = 13"cat8'2spcs\0
// now vars->ind --> beginning of 8' and vars->end --> " before 2'
// TODO: I now need to make vars->ind = vars->end+1 only in cases when end points to the closing quote
// """""""""""""'cat"''''''''  "'' main.c
int ft_command_first(t_input *input, t_vars *vars)
{
    char *s;

    ft_save_cmd(input, s, vars); // NOTE: if its command or its option, save to the vec.
    free(s);
    s = ft_next_string(vars, 0);
    if (!s)
	return (0);
    return (1);
}

