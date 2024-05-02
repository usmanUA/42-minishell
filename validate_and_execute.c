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

int    ft_validate_execute(t_shell *shell)
{
        int     cmd_flag;
        t_pipex pipex;
        t_input *input;

        pipex.idx = -1;
        pipex.status = 0;
        pipex.infile = -42;
        cmd_flag = GREEN;
        if (!vec_new(shell->info, 1, sizeof(int)))
            return (0);
        while (++pipex.idx < shell->pipes->len - 1)
        {
            input = *(t_input **)vec_get(shell->pipes, pipex.idx);
            if (*input->file_flag == RED)
                    continue;
            if (!ft_validate_commands(input, shell->info, shell->envp))
                return (0); // NOTE: malloc fail
            cmd_flag = *(int *)vec_get(shell->info, pipex.idx);
            if (cmd_flag == GREEN)
                ft_processes(input, &pipex, shell->envp);
         }
        input = *(t_input **)vec_get(shell->pipes, pipex.idx);
        if (*input->file_flag == RED)
                return (0);
        if (!ft_validate_commands(input, shell->info, shell->envp))
            return (0); // NOTE: malloc fail
        cmd_flag = *(int *)vec_get(shell->info, pipex.idx);
        if (cmd_flag == GREEN)
            ft_execute_last_cmd(input, &pipex, shell->envp);
        return (1);
}
