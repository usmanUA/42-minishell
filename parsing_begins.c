/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_user.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:55:03 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/16 13:55:05 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "miniwell.h"

void	ft_init_vars(t_vars *vars)
{
    // NOTE: Initialize vars struct
    vars->ind = 0;
    vars->end = 0;
    vars->len = 0;
    vars->fd = 0;
    vars->qontinue = 0;
    vars->s_quote = 0;
    vars->d_quote = 0;
    vars->increment = 0;
    vars->stop = 0;
    vars->file_error = 0;
    vars->input_line = NULL;
    vars->redir_count = NULL;
    vars->redirect = NULL;
}

void ft_index_after_spaces(t_vars *vars)
{
    // NOTE: SKIPS all the spaces and moves the pointer to the non-space character 
    while (vars->input_line[vars->end] && ft_isspace(vars->input_line[vars->end]))
	vars->end++;
    vars->ind = vars->end; // NOTE: index updates here
    //                    '\0'
}

void	ft_shift_pointer(t_vars *vars)
{
    // NOTE: moves the pointer either to the next | or null-terminator whatever comes first in the char *
    vars->ind = vars->end;
    while (vars->ind < vars->len)
    {
	if (vars->input_line[vars->ind] == '|') // NOTE: if encounters | stops.
	{
	    vars->ind++;
	    return ;
	}
	vars->ind++;
    }
}

int ft_redirection(t_vars *vars)
{
    // NOTE: checks the operator-first case
    int ind;

    ind = vars->ind;
    if (vars->input_line[ind] == '<' || vars->input_line[ind] == '>')
	return (1);
    while (ft_isdigit(vars->input_line[ind]))
	ind++;
    if (vars->input_line[ind] == '<' || vars->input_line[ind] == '>')
    {
	vars->fd = 1;
	return (1);
    }
    return (0);
}

static int ft_parsing_action(t_input **input, t_vec *cmd, t_vars *vars)
{
    // NOTE: Loops until either '\0' or '|'
    // parses everything in between
    // moves the pointer next to '|' if encountered
    char c;

    c = vars->input_line[vars->ind]; 
    while (c != '\0' && c != '|')
    {
	if (ft_redirection(vars))
	{
	    if (!ft_handle_redirects(input, vars)) // TODO: look for error handling 
		return (0);
	}
	else if (c != '\0' && c != '|')
	{
	    if (!ft_save_cmd(cmd, vars)) // TODO: look for error handling 
		return (0);
	}
	ft_index_after_spaces(vars);
	c = vars->input_line[vars->ind]; 
    }
    if (vars->input_line[vars->ind] == '|')
    {
	vars->end++;
	ft_index_after_spaces(vars);
    }
    return (1);
}

static int ft_parse_command_line(t_input **input, t_vars *vars)
{
    // NOTE: PARSES the WHOLE command line into the following two vectors
    int *file_flag;
    t_vec *cmd;
    char *null;

    null = NULL;
    cmd = (t_vec *)malloc(sizeof(t_vec));
    if (!cmd)
	return (0);
    if (!vec_new(cmd, 2, sizeof(char **))) // NOTE: Init a vec and allocate some mem for command
	return (0); // NOTE: malloc fail
    file_flag = (int *)malloc(sizeof(int));
    if (!file_flag)
	return (0);
    *file_flag = GREEN;
    (*input)->file_flag = file_flag;
    if (!ft_parsing_action(input, cmd, vars)) // TODO: Handle errors correctly
	return (0);
    if (!vec_push(cmd, &null))
	return (0); // NOTE: malloc fail
    // NOTE: saving addresses of cmd and redirect to input
    (*input)->cmd = cmd;
    return (1);
}

void ft_count_redirs(t_vars *vars, t_redir_count *redir_count)
{
    int ind;
    char c;

    ind = vars->ind;
    c = vars->input_line[ind];
    while (c != '\0' && c!= '|')
    {
	if (!ft_strncmp(&vars->input_line[ind], "<<", 2))
	{
	    ind += 2;
	    redir_count->in_redir++;
	}
	else if (!ft_strncmp(&vars->input_line[ind], ">>", 2))
	{
	    ind += 2;
	    redir_count->out_redir++;
	}
	else if (!ft_strncmp(&vars->input_line[ind], "<", 1))
	{
	    ind++;
	    redir_count->in_redir++;
	}
	else if (!ft_strncmp(&vars->input_line[ind], ">", 1))
	{
	    ind++;
	    redir_count->out_redir++;
	}
	else
	    ind++;
	c = vars->input_line[ind];
    }
}

void	ft_zero_redirects(t_redir_count *redir_count)
{
    redir_count->out_redir = 0;
    redir_count->in_redir = 0;
}

int ft_save_input(t_vec *pipes, t_vars *vars)
{
    // NOTE: PARSE EVERYTHING
    t_input *input;
    t_redir_count redir_count;


    ft_zero_redirects(&redir_count);
    ft_index_after_spaces(vars); 
    while (vars->input_line[vars->ind] != '\0') // NOTE: loop over the whole user input line
    {
	ft_count_redirs(vars, &redir_count);
	vars->redir_count = &redir_count;
	input = malloc(sizeof(t_input)); // NOTE: executed in the very beg. or the beg. of every pipe (|) if any
	if (!input)
	    return (0);
	input->new_fds = NULL;
	input->orig_fds = NULL;
	input->fds_info = NULL;
	if (!ft_parse_command_line(&input, vars)) // TODO: Handle errors correctly
	    return (0); // WARN: Handle malloc
	if (!vec_push(pipes, &input))
	    return (0); // NOTE: malloc fail
	ft_zero_redirects(&redir_count);
    }
    return (1);
}
