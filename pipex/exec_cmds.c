/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:32:08 by uahmed            #+#    #+#             */
/*   Updated: 2024/03/15 16:28:30 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	ft_child_process(t_pipex *ppx, int *fds, int pair, char **envp)
{
	if (pair == 0)
	{
		dup2(ppx->fd_in, STDIN_FILENO);
		close(ppx->fd_in);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		execve(ppx->cmds[0], ppx->cmd_args[0], envp);
		ft_exit_error(ppx, 0, EXIT_FAILURE, 0);
	}
	else
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		dup2(ppx->fd_out, STDOUT_FILENO);
		close(ppx->fd_out);
		execve(ppx->cmds[1], ppx->cmd_args[1], envp);
		ft_exit_error(ppx, 0, EXIT_FAILURE, 0);
	}
}

static void	ft_first_child(t_pipex *ppx, int *fds, char **envp)
{
	close(fds[0]);
	if (!ft_pair_failed(ppx, -1, 0))
		ft_child_process(ppx, fds, 0, envp);
	else
		close(fds[1]);
}

static void	ft_second_child(t_pipex *ppx, int *fds, int *status, char **envp)
{
	if (ppx->fdout_failed)
		ft_filerror(ppx->fdout_errno, ppx, 0, 1);
	if (!ft_pair_failed(ppx, 1, 1))
	{
		ppx->pids[1] = fork();
		if (ppx->pids[1] == -1)
			ft_exit_error(ppx, 1, EXIT_FAILURE, 1);
		if (ppx->pids[1] == 0)
			ft_child_process(ppx, fds, 1, envp);
		else
		{
			close(fds[0]);
			waitpid(ppx->pids[1], status, 0);
		}
	}
	else
	{
		close(fds[0]);
		*status = ft_pair_failed(ppx, 1, 1);
	}
}

int	ft_execute(t_pipex *ppx, char **envp)
{
	int	fds[2];
	int	pips;
	int	status;

	status = 0;
	pips = pipe(fds);
	if (pips == -1)
		ft_exit_error(ppx, 1, EXIT_FAILURE, 0);
	ppx->pids[0] = fork();
	if (ppx->pids[0] == -1)
		ft_exit_error(ppx, 1, EXIT_FAILURE, 0);
	if (ppx->pids[0] == 0)
		ft_first_child(ppx, fds, envp);
	else
	{
		close(fds[1]);
		ft_second_child(ppx, fds, &status, envp);
		waitpid(ppx->pids[0], NULL, 0);
	}
	if (status != 127 && status != 126 && status != 1)
		status = ft_status(status);
	ft_free_pipex(ppx);
	return (status);
}
