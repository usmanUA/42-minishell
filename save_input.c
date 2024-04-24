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

void	ft_init_vars(t_vars *vars)
{
    // NOTE: Initialize vars struct
    vars->ind = -1;
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

static size_t	ft_user_ends(char *s)
{
    // NOTE: NOT used function
    while (*s)
    {
	if (!ft_isspace(*s))
	    return (0);
	s++;
    }
    return (1);
}

int ft_index_after_spaces(t_vars *vars)
{
    // NOTE: SKIPS all the spaces and moves the pointer to the non-space character 
    // if there's no non-space character after the space/spaces returns false
    int ind;
    int end;

    ind = 0;
    end = vars->end;
    if (ft_space_until_end(&vars->input_line[vars->end]))
	return (0);
    while (ft_isspace(vars->input_line[vars->end+ind]))
	ind++;
    vars->ind = vars->end + ind; // NOTE: index updates here
    return (1);
}


// static int ft_redirect_operator(char c)
// {
//     // NOTE: returns true if the current string is one of the redirect operators
// 	return (1);
//     if (!ft_strncmp(c, "<<", 2) || !ft_strncmp(c, ">>", 2))
// 	return (1);
//     return (0);
// }

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

int ft_save_input(t_vec *pipes, t_vars *vars)
{
    // NOTE: PARSE EVERYTHING
    t_input *input;

    ft_index_after_spaces(vars); 
    while (++vars->ind < vars->len) // NOTE: loop over the whole user input line
    {
	input = malloc(sizeof(t_input)); // NOTE: executed in the very beg. or the beg. of every pipe (|) if any
	if (!input)
	    return (0);
	if (ft_redirection(vars))
	{
	    if (!ft_operator_first(input, vars)) // TODO: look for error handling 
		continue ;
	}
	else 
	{
	    if (!ft_command_first(input, vars)) // TODO: look for error handling 
		continue ;
	}
	if (!vec_push(pipes, input))
	    return (0); // NOTE: malloc fail
    }
    return (1);
}
