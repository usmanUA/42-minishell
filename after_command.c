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

int ft_save_cmd(t_input *input, char *s, t_vars *vars)
{
    // NOTE: PARSE command and its options if there are any
    t_vec cmd;

    if (!vec_new(&cmd, 2, sizeof(char *))) // NOTE: Initialize a vector and allocate some mem for command
	return (0);
    return (1);
}

int ft_after_command(t_input *input, char *s, t_vars *vars)
{

    ft_save_cmd(input, s, vars); // NOTE: if its command or its option, save to the vec.
    s = ft_next_string(vars, 0);
    if (!s)
	return (0);
    return (1);
}

