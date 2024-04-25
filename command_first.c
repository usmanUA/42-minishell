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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

int ft_save_cmd(t_vec *cmd, t_vars *vars)
{
    // NOTE: PARSE command and its options if there are any
    char *s;
    char *temp;

    if (vars->input_line[vars->ind] == '\"' || vars->input_line[vars->ind] == '\'')
	ft_skip_quotes(vars);
    s = ft_next_string(vars, COMMAND);
    printf("%s\n", s);
    if (!s)
	return (0); // NOTE: Either malloc fail or all spaces until '\0'
    vars->ind = vars->end;
    if (vars->increment)// WARN: adding 1 here works for all cases?
	vars->ind++;
    vars->increment = 0;
    while (vars->qontinue)
    {
// PERF:     ""'"""""""""""'cat"''''''''  "'' main.c '\0'
	temp = s;
	if (vars->input_line[vars->ind] == '\"' || vars->input_line[vars->ind] == '\'')
	    ft_skip_quotes(vars);
	// WARN: make sure the stop thing (space after quotes end case) works 
	if (vars->stop)
	{
	    vars->end = vars->ind;
	    break ;
	}
	s = ft_strjoin(s, ft_next_string(vars, COMMAND));
	if (!s)
	{
	    free(temp);
	    return (0);
	}
	free(temp);
	vars->ind = vars->end;
	if (vars->increment)
	    vars->ind++; 
	vars->increment = 0;
    }
    if (!vec_push(cmd, s))
    {
	free(s);
	return (0);
    }
    return (1);
}

int ft_follow_first_command_operator(t_vec *cmd, t_vec *redirect, t_vars *vars)
{
    char c;

    c = vars->input_line[vars->ind]; 
    while (c != '\0' && c != '|')
    {
	ft_index_after_spaces(vars);
	c = vars->input_line[vars->ind]; 
	if (ft_redirection(vars))
	{
	    if (!ft_handle_redirects(redirect, vars)) // TODO: look for error handling 
		return (0);
	}
	else if (c != '\0' && c != '|')
	{
	    if (!ft_save_cmd(cmd, vars)) // TODO: look for error handling 
		return (0);
	}
    }
    if (vars->input_line[vars->ind] == '|')
	vars->ind++;
    return (1);
}
// """""""""""""'cat"''''''''  "'' main.c
int ft_command_first(t_input *input, t_vars *vars)
{
    // NOTE: FIRST saves the command and then moves till the end
    t_vec *cmd;
    t_vec *redirect;

    cmd = (t_vec *)malloc(sizeof(t_vec));
    if (!cmd)
	return (0);
    redirect = (t_vec *)malloc(sizeof(t_vec));
    if (!redirect)
	return (0);
    if (!vec_new(cmd, 2, sizeof(char *))) // NOTE: Initialize a vec and allocate some mem for command
	return (0); // NOTE: malloc fail
    if (!ft_save_cmd(cmd, vars)) // NOTE: if its command or its option, save to the vec.
	return (0); // NOTE: only malloc fail?
    // NOTE: NOW: expect either options or redirect operators
    if (!vec_new(redirect, 2, sizeof(t_redirect *)))
	return (0);
    if (!ft_follow_first_command_operator(cmd, redirect, vars))
	return (0); // TODO: look for error handling 
    // NOTE: saving address (in stack mem)
    // It can be malloc if needed be
    input->cmd = cmd;
    input->redirect = redirect;
    return (1);
}

