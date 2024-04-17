/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:00:17 by uahmed            #+#    #+#             */
/*   Updated: 2024/03/15 16:09:04 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	ft_write_error(int errnu, char *filename)
{
	if (!errnu)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd("No such file or directory", 2);
		return ;
	}
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errnu), 2);
}

void	ft_filerror(int errnu, t_pipex *pipex, int infile, int write)
{
	if (infile)
	{
		ft_write_error(errnu, pipex->infile);
		pipex->fdin_failed = 1;
	}
	else
	{
		if (write)
			ft_write_error(errnu, pipex->outfile);
		pipex->fdout_failed = 1;
	}
}

void	ft_cmd_error(char *cmd, int permission, int file_exist)
{
	if (!file_exist)
	{
		ft_write_error(errno, cmd);
		return ;
	}
	ft_putstr_fd("pipex: ", 2);
	if (permission == 1)
	{
		if (cmd[0])
			ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": command not found", 2);
		return ;
	}
	if (permission == 2)
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": is a directory", 2);
		return ;
	}
	perror(cmd);
}

void	ft_exit_error(t_pipex *pipex, int free, int status, int wait_index)
{
	if (wait_index)
		ft_wait_pids(pipex, wait_index);
	if (free)
	{
		perror("pipex: ");
		ft_free_pipex(pipex);
	}
	exit(status);
}

int	ft_notwrite_cmderror(t_pipex *ppx)
{
	if (!ppx->idx && ppx->fdin_failed)
		return (1);
	if (ppx->idx == ppx->tot_cmds - 1 && ppx->fdout_failed)
		return (1);
	return (0);
}
