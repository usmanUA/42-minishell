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

int ft_after_operator(t_input *input, char *s, t_vars *vars)
{
    // NOTE: PARSE everthing coming after the redirect operator.
    // FOLLOWS after_redirect_operator PATTERN
    if (ft_syntax_error(vars, vars->len - vars->end)) // NOTE: if there's syntax error after the |
    {
	ft_shift_pointer(vars); // NOTE: move to the end or to the next |
	return (1);
    }
    if (ft_handle_redirects(input, s, vars)) // NOTE: open files and save their fds to vec. operators are followed by files 
    {
	// WARN: input not pushed here after saving redirect
    }
    else
	ft_next_pipe_null(vars);
    return (1);
}
