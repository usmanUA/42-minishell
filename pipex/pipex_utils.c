/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:02:38 by uahmed            #+#    #+#             */
/*   Updated: 2024/03/15 16:34:23 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (1);
}

static int	ft_failure_status(int file_status, char *cmd)
{
	if (file_status)
		return (1);
	if (!cmd)
		return (127);
	if (!ft_strncmp(cmd, "NO_EXEC", 7) || !ft_strncmp(cmd, "DIR", 3))
		return (126);
	return (0);
}

int	ft_pair_failed(t_pipex *ppx, int cmd_num, int i)
{
	if (i && i != ppx->tot_cmds - 1 && ppx->tot_cmds != 2)
		cmd_num = 0;
	if (cmd_num == -1)
		return (ft_failure_status(ppx->fdin_failed, ppx->cmds[i]));
	else if (cmd_num == 1)
		return (ft_failure_status(ppx->fdout_failed, ppx->cmds[i]));
	return (ft_failure_status(0, ppx->cmds[i]));
}

void	ft_free_pipex(t_pipex *pipex)
{
	int	ind;

	ind = -1;
	if (pipex->cmd_args[0])
	{
		while (pipex->cmd_args[++ind])
			ft_free(pipex->cmd_args[ind], 0);
		free(pipex->cmd_args);
	}
	if (pipex->paths)
		ft_free(pipex->paths, 0);
	if (pipex->cmds)
		ft_free(pipex->cmds, pipex->idx + 1);
	if (pipex->pids)
		free(pipex->pids);
	if (pipex->fd_in > -1)
		close(pipex->fd_in);
	if (pipex->fd_out > -1)
		close(pipex->fd_out);
}

void	ft_initialize_pipex(t_pipex *pipex, int tot_cmds, char *infile,
		char *outfile)
{
	pipex->fd_in = -2;
	pipex->fdin_failed = 0;
	pipex->fd_out = -2;
	pipex->fdout_failed = 0;
	pipex->idx = -1;
	pipex->start = 2;
	pipex->here_doc = false;
	pipex->infile = infile;
	pipex->outfile = outfile;
	pipex->fdout_errno = 0;
	pipex->paths = NULL;
	pipex->cmds = NULL;
	pipex->cmd_args = NULL;
	pipex->pids = (int *)malloc((tot_cmds) * sizeof(int));
	if (!pipex->pids)
		ft_exit_error(pipex, 1, EXIT_FAILURE, 0);
	pipex->tot_cmds = tot_cmds;
}
