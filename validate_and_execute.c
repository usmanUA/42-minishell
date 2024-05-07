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

static	int	ft_validate_exec_last_child(t_pipex *pipex, t_shell *shell)
{
	if (ft_validate_commands(pipex->input, shell) == MALLOC_FAIL)
		return (MALLOC_FAIL); // NOTE: malloc fail
	pipex->cmd_flag = *(int *)vec_get(shell->info, pipex->idx);
	shell->status = pipex->cmd_flag;
	if (pipex->cmd_flag == GREEN)
	{
		ft_execute_last_cmd(pipex->input, pipex, shell->envp);
		shell->status = (int)pipex->status;
	}
	return (MALLOC_SUCCESS);
}

static	int	ft_validate_exec_childs(t_pipex *pipex, t_shell *shell, int *file_flag)
{
	pipex->input = *(t_input **)vec_get(shell->pipes, pipex->idx);
	if (*pipex->input->file_flag == BROWN)
		*file_flag = BROWN;
	if (ft_validate_commands(pipex->input, shell) == MALLOC_FAIL)
		return (MALLOC_FAIL); // NOTE: malloc fail
	pipex->cmd_flag = *(int *)vec_get(shell->info, pipex->idx);
	if (pipex->cmd_flag == GREEN)
		ft_processes(pipex->input, pipex, shell->envp);
	return (MALLOC_SUCCESS);
}

int	ft_validate_execute(t_shell *shell)
{
	t_pipex	pipex;
	int	file_flag;

	file_flag = GREEN;
	ft_init_pipex(&pipex);
	if (!vec_new(shell->info, 1, sizeof(int)))
		return (MALLOC_FAIL);
	while (++pipex.idx < shell->pipes->len - 1)
	{
		if (ft_validate_exec_childs(&pipex, shell, &file_flag) == MALLOC_FAIL)
			return (MALLOC_FAIL);
		if (file_flag == BROWN)
			continue;
	}
	pipex.input = *(t_input **)vec_get(shell->pipes, pipex.idx);
	if (*pipex.input->file_flag == BROWN)
	{
		shell->status = BROWN;
		return (FILE_FAIL);
	}
	if (ft_validate_exec_last_child(&pipex, shell) == MALLOC_FAIL)
		return (MALLOC_FAIL); // NOTE: malloc fail
	return (MALLOC_SUCCESS);
}

