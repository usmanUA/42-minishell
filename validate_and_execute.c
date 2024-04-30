/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_and_execute.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:45:53 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/30 15:45:54 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "miniwell.h"
#include "vec/vec.h"

void    ft_validate_execute(t_shell *shell)
{
    t_vec   info;
    int     ind;
    int     cmd_status;

    ind = -1;
    if (!vec_new(&info, 1, sizeof(int)))
        return ;
    while (++ind < shell->pipes.len)
    {
        ft_validate_commands(shell, ind);
        cmd_status = *(int *)vec_get(&shell->info, ind);
        if (cmd_status == GREEN)
            ft_execute(&shell->pipes);
    }
}
