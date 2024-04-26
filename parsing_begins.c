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
#include "libft/libft.h"
#include "miniwell.h"
#include "vec/vec.h"
#include <stdio.h>
#include <stdlib.h>

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
    vars->input_line = NULL;
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

static int ft_init_fd_vecs(t_vec **in_orig_fd, t_vec **in_new_fd, t_vec **out_orig_fd, t_vec **out_new_fd)
{
    *in_orig_fd = (t_vec *)malloc(sizeof(t_vec)); 
    if (!in_orig_fd)
	return (0);
    if (!vec_new(*in_orig_fd, 1, sizeof(int)))
	return (0);
    *in_new_fd = (t_vec *)malloc(sizeof(t_vec)); 
    if (!in_new_fd)
	return (0);
    if (!vec_new(*in_new_fd, 1, sizeof(int)))
	return (0);
    *out_orig_fd = (t_vec *)malloc(sizeof(t_vec)); 
    if (!out_orig_fd)
	return (0);
    *out_new_fd = (t_vec *)malloc(sizeof(t_vec)); 
    if (!out_new_fd)
	return (0);
    return (1);
}

static int ft_parsing_action(t_vec *cmd, t_redirect *redirect, t_vars *vars)
{
    // NOTE: Loops until either '\0' or '|'
    // parses everything in between
    // moves the pointer next to '|' if encountered
    char c;
    t_vec *in_orig_fd;
    t_vec *in_new_fd;
    t_vec *out_orig_fd;
    t_vec *out_new_fd;

    if (!ft_init_fd_vecs(&in_orig_fd, &in_new_fd, &out_orig_fd, &out_new_fd))
	return (0);
    redirect->in_orig_fd = in_orig_fd;
    redirect->in_new_fd = in_new_fd;
    redirect->out_orig_fd = out_orig_fd;
    redirect->out_new_fd = out_new_fd;
    c = vars->input_line[vars->ind]; 
    while (c != '\0' && c != '|')
    {
	if (ft_redirection(vars))
	{
	    if (!ft_handle_redirects(redirect, vars)) // TODO: look for error handling 
	    {
		free(in_orig_fd);
		vec_free(in_orig_fd);
		free(in_new_fd);
		vec_free(in_new_fd);
		free(out_orig_fd);
		vec_free(out_orig_fd);
		free(out_new_fd);
		vec_free(out_new_fd);
		return (0);
	    }
	}
	else if (c != '\0' && c != '|')
	{
	    if (!ft_save_cmd(cmd, vars)) // TODO: look for error handling 
	    {
		free(in_orig_fd);
		vec_free(in_orig_fd);
		free(in_new_fd);
		vec_free(in_new_fd);
		free(out_orig_fd);
		vec_free(out_orig_fd);
		free(out_new_fd);
		vec_free(out_new_fd);
		return (0);
	    }
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

static int ft_parse_command_line(t_input *input, t_vars *vars)
{
    // NOTE: PARSES the WHOLE command line into the following two vectors
    t_vec *cmd;
    t_redirect *redirect;

    cmd = (t_vec *)malloc(sizeof(t_vec));
    if (!cmd)
	return (0);
    redirect = (t_redirect *)malloc(sizeof(t_redirect));
    if (!redirect)
	return (0);
    if (!vec_new(cmd, 2, sizeof(char *))) // NOTE: Init a vec and allocate some mem for command
	return (0); // NOTE: malloc fail
    if (!ft_parsing_action(cmd, redirect, vars)) // TODO: Handle errors correctly
    {
	free(cmd);
	vec_free(cmd);
	free(redirect);
	return (0);
    }
    // NOTE: saving addresses of cmd and redirect to input
    input->cmd = cmd;
    input->redirect = redirect;
    return (1);
}

int ft_save_input(t_vec *pipes, t_vars *vars)
{
    // NOTE: PARSE EVERYTHING
    t_input *input;

    ft_index_after_spaces(vars); 
    while (vars->input_line[vars->ind] != '\0') // NOTE: loop over the whole user input line
    {
	input = malloc(sizeof(t_input)); // NOTE: executed in the very beg. or the beg. of every pipe (|) if any
	if (!input)
	    return (0);
	if (!ft_parse_command_line(input, vars)) // TODO: Handle errors correctly
	    return (0); // WARN: Handle malloc
	if (!vec_push(pipes, input))
	    return (0); // NOTE: malloc fail
    }
    return (1);
}
