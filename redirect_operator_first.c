/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   after_redirect_operator.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:59:02 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/18 13:59:04 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "miniwell.h"
#include <stdlib.h>

int ft_operator_first(t_input *input, t_vars *vars)
{
    // NOTE: PARSE everthing coming after the redirect operator.
    // FOLLOWS operator_first PATTERN
    char *s;
    char *fd;

    if (vars->fd)
    {
	fd = ft_next_string(vars, FD); // NOTE: str in heap, vars->ind points ->(opertr)<-vars->end 
	if (!fd)
	    return (0);
    }
    s = ft_next_string(vars, REDIRECT); // NOTE: str in heap, vars->ind points ->(opertr)<-vars->end 
    if (!s)
	return (0); ; // NOTE: if malloc fails (am I handling it in a correct way?)
    if (ft_special_char(vars->input_line[vars->end]))
    {
	ft_shift_pointer(vars); // NOTE: points to \0 or the character followed by |
	ft_token_error(vars->input_line[vars->end]);
	vars->end = vars->ind;
	free(s);
	free(fd);
	return (0);
    }
    if (!ft_handle_redirects(input, s, fd, vars))// NOTE: open files and saves their fds to vec.
    {
	free(s);
	free(fd);
	return (0);
    }
    free(s);
    s = ft_next_string(vars, COMMAND);
    if (!s)
	return (0);
    if (!ft_save_cmd(input, s, vars))
    {
	free(s);
	s = ft_next_string(vars, COMMAND);
	if (!s)
	    return (0);
	// WARN: input not pushed here after saving redirect
    }
    return (0);// TODO: return exit status
}
