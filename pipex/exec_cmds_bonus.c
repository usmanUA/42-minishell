/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:32:08 by uahmed            #+#    #+#             */
/*   Updated: 2024/03/15 16:13:32 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	ft_exec_child(t_pipex *ppx, int *fds, char **envp, int ind)
{
	dup2(fds[1], STDOUT_FILENO);
	close(fds[1]);
	execve(ppx->cmds[ind], ppx->cmd_args[ind], envp);
	ft_exit_error(ppx, 0, EXIT_FAILURE, 0);
}

static void	ft_child(t_pipex *ppx, int *fds, char **envp, int ind)
{
	if (ind == 0)
	{
		close(fds[0]);
		dup2(ppx->fd_in, STDIN_FILENO);
		close(ppx->fd_in);
		ft_exec_child(ppx, fds, envp, ind);
	}
	else
	{
		close(fds[0]);
		ft_exec_child(ppx, fds, envp, ind);
	}
}

static void	ft_last_child(t_pipex *ppx, int *status, char **envp)
{
	if (ppx->fdout_failed)
		ft_filerror(ppx->fdout_errno, ppx, 0, 1);
	if (!ft_pair_failed(ppx, 1, ppx->tot_cmds - 1))
	{
		dup2(ppx->fd_out, STDOUT_FILENO);
		close(ppx->fd_out);
		execve(ppx->cmds[ppx->tot_cmds - 1], ppx->cmd_args[ppx->tot_cmds - 1],
			envp);
	}
	else
	{
		close(ppx->fd_out);
		*status = ft_pair_failed(ppx, -1, ppx->tot_cmds - 1);
	}
}

static void	ft_processes(t_pipex *ppx, int ind, char **envp)
{
	int	fds[2];

	if (pipe(fds) == -1)
		ft_exit_error(ppx, 1, EXIT_FAILURE, ind);
	ppx->pids[ind] = fork();
	if (ppx->pids[ind] == -1)
		ft_exit_error(ppx, 1, EXIT_FAILURE, ind);
	if (ppx->pids[ind] == 0)
	{
		if (!ft_pair_failed(ppx, -1, ind))
			ft_child(ppx, fds, envp, ind);
		else
		{
			close(fds[0]);
			close(fds[1]);
			ft_exit_error(ppx, 0, EXIT_FAILURE, 0);
		}
	}
	else
	{
		close(fds[1]);
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
	}
}

int	ft_execute_bonus(t_pipex *ppx, char **envp)
{
	int	i;
	int	status;

	i = -1;
	status = 0;
	while (++i < ppx->tot_cmds - 1)
		ft_processes(ppx, i, envp);
	ppx->pids[ppx->tot_cmds - 1] = fork();
	if (ppx->pids[ppx->tot_cmds - 1] == -1)
		ft_exit_error(ppx, 1, EXIT_FAILURE, i);
	if (ppx->pids[ppx->tot_cmds - 1] == 0)
		ft_last_child(ppx, &status, envp);
	else
		waitpid(ppx->pids[ppx->tot_cmds - 1], &status, 0);
	if (status != 127 && status != 126)
		status = ft_status(status);
	ft_free_pipex(ppx);
	return (status);
}
