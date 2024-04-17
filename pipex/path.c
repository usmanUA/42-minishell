/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 10:51:13 by uahmed            #+#    #+#             */
/*   Updated: 2024/03/15 16:34:08 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_give_path(char **envp)
{
	char	*path;

	path = NULL;
	while (*envp)
	{
		if (!ft_strncmp(*envp, "PATH=", 5))
		{
			path = *envp;
			break ;
		}
		envp++;
	}
	if (!path)
		return (path);
	return (&path[5]);
}

void	ft_make_command(t_pipex *ppx)
{
	char	*cmd_path;
	int		i;

	i = -1;
	while (ppx->paths[++i])
	{
		cmd_path = ft_join_path(ppx->paths[i], ppx);
		if (!access(cmd_path, F_OK))
		{
			if (access(cmd_path, X_OK) == -1)
			{
				ft_cmd_error(ppx->cmd_args[ppx->idx][0], 0, 1);
				ppx->cmds[ppx->idx] = ft_strdup("NO_EXEC");
			}
			else
				ppx->cmds[ppx->idx] = cmd_path;
			return ;
		}
		free(cmd_path);
	}
	ppx->cmds[ppx->idx] = NULL;
	if (ft_notwrite_cmderror(ppx))
		return ;
	ft_cmd_error(ppx->cmd_args[ppx->idx][0], 1, 1);
}

char	*ft_join_path(char *path, t_pipex *ppx)
{
	char	*path_to_cmd;
	char	*cmd_path;

	path_to_cmd = ft_strjoin(path, "/");
	if (!path_to_cmd)
		ft_exit_error(ppx, 1, EXIT_FAILURE, 0);
	cmd_path = ft_strjoin(path_to_cmd, ppx->cmd_args[ppx->idx][0]);
	if (!cmd_path)
	{
		free(path_to_cmd);
		ft_exit_error(ppx, 1, EXIT_FAILURE, 0);
	}
	if (path_to_cmd)
		free(path_to_cmd);
	return (cmd_path);
}

void	ft_handle_absolute(t_pipex *ppx)
{
	int	fd;

	fd = -2;
	if (!access(ppx->cmd_args[ppx->idx][0], F_OK))
	{
		fd = open(ppx->cmd_args[ppx->idx][0], O_DIRECTORY);
		if (fd != -1)
		{
			close(fd);
			ft_cmd_error(ppx->cmd_args[ppx->idx][0], 2, 1);
			ppx->cmds[ppx->idx] = ft_strdup("DIR");
		}
		else if (access(ppx->cmd_args[ppx->idx][0], X_OK) == -1)
		{
			ft_cmd_error(ppx->cmd_args[ppx->idx][0], 0, 1);
			ppx->cmds[ppx->idx] = ft_strdup("NO_EXEC");
		}
		else
			ppx->cmds[ppx->idx] = ft_strdup(ppx->cmd_args[ppx->idx][0]);
	}
	else
	{
		ppx->cmds[ppx->idx] = NULL;
		ft_cmd_error(ppx->cmd_args[ppx->idx][0], 0, 0);
	}
}

void	ft_handle_relative(t_pipex *ppx)
{
	if (!ppx->paths)
	{
		ppx->cmds[ppx->idx] = NULL;
		if (!ft_notwrite_cmderror(ppx))
			ft_cmd_error(ppx->cmd_args[ppx->idx][0], 1, 0);
		return ;
	}
	ft_make_command(ppx);
}
