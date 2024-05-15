/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:15:13 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/29 14:15:24 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include <stdio.h>

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

static int	ft_check_dir(t_pipex *pipex)
{
	int	fd;

	fd = -2;
	fd = open(pipex->command, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		ft_cmd_error(pipex->command, 2, 1); // NOTE: is a directory
		pipex->cmd_flag = YELLOW;
		// TODO: return relevant exit status 126
		close(fd);
		return (INVALID);
	}
	close(fd);
	return (VALID);
}

static int	ft_check_command(char *command, t_pipex *pipex, int check_dir)
{
	if (!access(command, F_OK))
	{
		if (check_dir == YES)
		{
			if (ft_check_dir(pipex) == INVALID)
				return (INVALID);
		}
		if (access(command, X_OK) == -1)
		{
			ft_cmd_error(pipex->command, 0, 1);
			pipex->cmd_flag = YELLOW;
			return (INVALID);
		}
	}
	else
		return (INVALID);
	return (VALID);
}

void	ft_handle_absolute(t_shell *shell, t_pipex *pipex)
{
	int	fd;

	fd = -2;
	ft_check_command(pipex->command, pipex, YES);
	// NOTE: if absolute command (given) does not exist
	if (pipex->cmd_flag == RED)
		ft_cmd_error(pipex->command, 0, 0); // TODO: define MACROS for const. values
}

static int	ft_make_command(t_pipex *pipex, char **paths)
{
	char	*cmd_path;
	int		i;

	i = -1;
	while (paths[++i])
	{
		cmd_path = ft_join_path(paths[i], pipex->command);
		if (!cmd_path)
			return (MALLOC_FAIL);
		if (ft_check_command(cmd_path, pipex, NO) == VALID)
		{
			// TODO: look for handling ft_memmove and ft_memcpy failures
			// WARN: make sure you're doing correctly
			vec_insert((*pipex->input)->cmd, &cmd_path, 0);
				// NOTE: make sure the address of cmd_path goes along
			vec_remove((*pipex->input)->cmd, 1);           
				// NOTE: overwrites the address of command (overwritten address is the pointer to the address of command)
			free(pipex->command);               
				// NOTE: free the command (command is the pointer to the first char of malloced string)
			pipex->cmd_flag = GREEN;
			break ;
		}
		else
			free(cmd_path);
	}
	return (MALLOC_SUCCESS);
}

void	ft_handle_relative(t_pipex *pipex, t_shell *shell, char **paths)
{
	pipex->cmd_flag = RED;
	if (ft_make_command(pipex, paths) == MALLOC_FAIL)
	{
		shell->vars->malloc_flag = RED;
		ft_free_prompt(shell, NO);
		return;
	}
	// NOTE: if it gets here it means command does not exits
	// TODO: return exit status 127
	if (pipex->cmd_flag == RED)
		ft_cmd_error(pipex->command, 1, 1);
}

void	ft_check_builtin(t_shell *shell, char *command, t_pipex *pipex)
{
	int	executable_info;

	executable_info = EXTERNAL;
	if (ft_strcmp(command, "cd") == 0)
		executable_info = CD;
	if (ft_strcmp(command, "echo") == 0)
		executable_info = MY_ECHO;
	if (ft_strcmp(command, "env") == 0)
		executable_info = ENV;
	if (ft_strcmp(command, "exit") == 0)
		executable_info = EXIT;
	if (ft_strcmp(command, "export") == 0)
		executable_info = EXPORT;
	if (ft_strcmp(command, "pwd") == 0)
		executable_info = PWD;
	if (ft_strcmp(command, "unset") == 0)
		executable_info = UNSET;
	pipex->exec_type = executable_info;
}

void	ft_validate_commands(t_pipex *pipex, t_shell *shell)
{
	char	**paths;

	paths = ft_split(ft_give_path(shell->envp), ':');
	if (!paths)
	{
		shell->vars->malloc_flag = RED;
		ft_free_prompt(shell, NO);
		return;
	}
	pipex->command = *(char **)vec_get((*pipex->input)->cmd, 0);
	if (ft_ispresent(pipex->command, '/'))
		ft_handle_absolute(shell, pipex);
	else
	{
		ft_check_builtin(shell, pipex->command, pipex);
		if (pipex->exec_type == EXTERNAL) 
			ft_handle_relative(pipex, shell, paths);
	}
	ft_free_splitted_paths(paths);
}
