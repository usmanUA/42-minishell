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
#include <stdio.h>

int    ft_validate_execute(t_shell *shell)
{
        t_pipex pipex;

        ft_init_pipex(&pipex);
        if (!vec_new(shell->info, 1, sizeof(int)))
            return (MALLOC_FAIL);
        while (++pipex.idx < shell->pipes->len - 1)
        {
            pipex.input = *(t_input **)vec_get(shell->pipes, pipex.idx);
            if (*pipex.input->file_flag == RED)
                    continue;
            if (ft_validate_commands(pipex.input, shell->info, shell->envp) == MALLOC_FAIL)
                return (MALLOC_FAIL); // NOTE: malloc fail
            pipex.cmd_flag = *(int *)vec_get(shell->info, pipex.idx);
            if (pipex.cmd_flag == GREEN)
                ft_processes(pipex.input, &pipex, shell->envp);
         }
        pipex.input = *(t_input **)vec_get(shell->pipes, pipex.idx);
        if (*pipex.input->file_flag == RED)
                return (0);
        if (ft_validate_commands(pipex.input, shell->info, shell->envp) == MALLOC_FAIL)
            return (MALLOC_FAIL); // NOTE: malloc fail
        pipex.cmd_flag = *(int *)vec_get(shell->info, pipex.idx);
        if (pipex.cmd_flag == GREEN)
            ft_execute_last_cmd(pipex.input, &pipex, shell->envp);
        return (MALLOC_SUCCESS);
}
