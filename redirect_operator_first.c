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
	ft_shift_pointer(vars); // NOTE: to \0 or the character followed by |
	free(fd);
	free(s);
	ft_token_error(vars->input_line[vars->end]);
	vars->end = vars->ind;
	return (0); // TODO: return exit status
    }
    if (!ft_handle_redirects(input, s, fd, vars)) // NOTE: open files and saves their fds to vec. operators are followed by files 
    {
	// WARN: input not pushed here after saving redirect
	ft_shift_pointer(vars); // NOTE: to \0 or the character followed by |
	free(fd);
	free(s);
	ft_token_error(vars->input_line[vars->end]);
	vars->end = vars->ind;
	return (0); // TODO: return exit status
    }
    free(s);
    free(fd);
    return (1);
}
