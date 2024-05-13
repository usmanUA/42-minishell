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
#include "minishell.h"

static	void	ft_validate_exec_last_child(t_pipex *pipex, t_shell *shell)
{
	int	cmd_flag;

	cmd_flag = GREEN;
	ft_validate_commands(pipex, shell);
	if (pipex->exec_type != EXTERNAL)
	{
		if (!vec_push(shell->info, &cmd_flag))
		{
			ft_free_all(shell, YES);
			exit(EXIT_FAILURE);
		}
	}
	if (pipex->exec_type == EXTERNAL)
		pipex->cmd_flag = *(int *)vec_get(shell->info, pipex->idx);
	shell->status = pipex->cmd_flag;
	if (pipex->cmd_flag == GREEN)
		ft_execute_last_cmd(pipex, shell);
}

void	ft_init_pids(t_shell *shell)
{
	t_vec	*pids;

	pids = (t_vec *)malloc(sizeof(t_vec));
	if (!pids)
	{
		ft_free_all(shell, YES);
		exit(EXIT_FAILURE);
	}
	if (!vec_new(pids, 1, sizeof(int)))
	{
		ft_free_all(shell, YES);
		exit(EXIT_FAILURE);
	}
	shell->pids = pids;
}

static	void	ft_validate_exec_childs(t_pipex *pipex, t_shell *shell)
{
	int	cmd_flag;

	cmd_flag = GREEN;
	ft_validate_commands(pipex, shell);
	if (pipex->exec_type != EXTERNAL)
	{
		if (!vec_push(shell->info, &cmd_flag))
		{
			ft_free_all(shell, YES);
			exit(EXIT_FAILURE);
		}
	}
	if (pipex->exec_type == EXTERNAL)
		pipex->cmd_flag = *(int *)vec_get(shell->info, pipex->idx);
	if (pipex->cmd_flag == GREEN)
		ft_processes(pipex, shell);
}

void	ft_validate_execute(t_shell *shell)
{
	t_pipex	pipex;

	ft_init_pipex(&pipex, shell->pipes->len - 1);
	if (pipex.tot_pipes > 0)
		ft_init_pids(shell);
	while (++pipex.idx < shell->pipes->len - 1)
	{
		pipex.input = *(t_input **)vec_get(shell->pipes, pipex.idx);
		if (*pipex.input->file_flag == BROWN)
			continue;
		ft_validate_exec_childs(&pipex, shell);
	}
	pipex.input = *(t_input **)vec_get(shell->pipes, pipex.idx);
	shell->status = *pipex.input->file_flag;
	if (shell->status == BROWN)
		return;
	ft_validate_exec_last_child(&pipex, shell);
}

