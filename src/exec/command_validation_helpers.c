/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_validation_helpers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 12:33:44 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/22 15:08:57 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_splitted_paths(char **paths)
{
	int	ind;

	ind = -1;
	while (paths[++ind])
		free(paths[ind]);
	free(paths);
}

char	*ft_join_path(char *path, char *command)
{
	char	*path_to_cmd;
	char	*cmd_path;

	path_to_cmd = NULL;
	path_to_cmd = ft_strjoin(path, "/");
	if (!path_to_cmd)
		return (NULL);
	cmd_path = ft_strjoin(path_to_cmd, command);
	if (!cmd_path)
	{
		free(path_to_cmd);
		return (NULL);
	}
	free(path_to_cmd);
	return (cmd_path);
}

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
		return (NULL);
	return (&path[5]);
}

int	ft_check_dir(t_pipex *pipex)
{
	int	fd;

	fd = -2;
	fd = open(pipex->command, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		ft_cmd_error(pipex->command, 2, 1);
		pipex->cmd_flag = YELLOW;
		close(fd);
		return (INVALID);
	}
	close(fd);
	return (VALID);
}
