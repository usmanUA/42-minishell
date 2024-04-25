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

// WARN: 0<"here" test cat
// bash: here: No such file or directory

int ft_operator_first(t_input *input, t_vars *vars)
{
    // NOTE: PARSE everthing coming after the redirect operator.
    // FOLLOWS operator-first PATTERN
    t_vec *cmd;
    t_vec *redirect;

    // NOTE: init a vec redirect and cmd every time a new pair starts
    cmd = (t_vec *)malloc(sizeof(t_vec));
    if (!cmd)
	return (0);
    redirect = (t_vec *)malloc(sizeof(t_vec));
    if (!redirect)
	return (0);
    if (!vec_new(redirect, 1, sizeof(t_redirect *)))
	return (0); // NOTE: malloc fail
    if (!ft_handle_redirects(redirect, vars))// NOTE: open files and saves their fds to vec.
	return (0); // WARN: its either malloc or file opening error, handle accordingly
    vars->ind = vars->end;
    // TODO: Bring a while loop here
    if (!ft_follow_first_command_operator(cmd, redirect, vars))
	return (0); // TODO: look for error handling 
    input->cmd = cmd;
    input->redirect = redirect;
    return (1);
}
