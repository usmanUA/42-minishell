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

	path_to_cmd = ft_strjoin(path, "/");
	if (!path_to_cmd)
		return (NULL);
	cmd_path = ft_strjoin(path_to_cmd, command);
	if (!cmd_path)
	{
		free(path_to_cmd);
		return (NULL);
	}
	if (path_to_cmd)
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

static int	ft_check_dir(char *command, int *cmd_flag)
{
	int	fd;

	fd = -2;
	fd = open(command, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		ft_cmd_error(command, 2, 1); // NOTE: is a directory
		*cmd_flag = YELLOW;
		// TODO: return relevant exit status 126
		close(fd);
		return (INVALID);
	}
	close(fd);
	return (VALID);
}

static int	ft_check_command(char *command, int check_dir, int *cmd_flag)
{
	if (!access(command, F_OK))
	{
		if (check_dir == YES)
		{
			if (ft_check_dir(command, cmd_flag) == INVALID)
				return (INVALID);
		}
		if (access(command, X_OK) == -1)
		{
			ft_cmd_error(command, 0, 1);
			*cmd_flag = YELLOW;
			return (INVALID);
		}
	}
	else
		return (INVALID);
	*cmd_flag = GREEN;
	return (VALID);
}

void	ft_handle_absolute(char *command, t_shell *shell, char ***paths)
{
	int	fd;
	int	cmd_flag;

	fd = -2;
	cmd_flag = RED;
	ft_check_command(command, YES, &cmd_flag);
	// NOTE: if absolute command (given) does not exist
	if (cmd_flag == RED)
	{
		ft_cmd_error(command, 0, 0); // TODO: define MACROS for const. values
	}
	if (!vec_push(shell->info, &cmd_flag))
	{
		ft_free_splitted_paths(*paths);
		ft_free_all(shell, YES);
		exit(EXIT_FAILURE);
	}
}

static int	ft_make_command(t_vec *cmd, char **command, char **paths,
		int *cmd_flag)
{
	char	*cmd_path;
	int		i;

	i = -1;
	while (paths[++i])
	{
		cmd_path = ft_join_path(paths[i], *command);
		if (!cmd_path)
			return (MALLOC_FAIL);
		if (ft_check_command(cmd_path, NO, cmd_flag) == VALID)
		{
			// TODO: look for handling ft_memmove and ft_memcpy failures
			// WARN: make sure you're doing correctly
			vec_insert(cmd, &cmd_path, 0);
				// NOTE: make sure the address of cmd_path goes along
			vec_remove(cmd, 1);           
				// NOTE: overwrites the address of command (overwritten address is the pointer to the address of command)
			free(*command);               
				// NOTE: free the command (command is the pointer to the first char of malloced string)
			*cmd_flag = GREEN;
			break ;
		}
		else
			free(cmd_path);
	}
	return (MALLOC_SUCCESS);
}

void	ft_handle_relative(t_vec *cmd, char **command, t_shell *shell, char ***paths)
{
	int	cmd_flag;

	cmd_flag = RED;
	if (ft_make_command(cmd, command, *paths, &cmd_flag) == MALLOC_FAIL)
	{
		ft_free_splitted_paths(*paths);
		ft_free_all(shell, YES);
		exit(EXIT_FAILURE);
	}
	// NOTE: if it gets here it means command does not exits
	// TODO: return exit status 127
	if (cmd_flag != GREEN)
		ft_cmd_error(*command, 1, 1);
	if (!vec_push(shell->info, &cmd_flag))
	{
		ft_free_splitted_paths(*paths);
		ft_free_all(shell, YES);
		exit(EXIT_FAILURE);
	}
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
	char	*command;

	paths = ft_split(ft_give_path(shell->envp), ':');
	if (!paths)
	{
		ft_free_all(shell, YES);
		exit(EXIT_FAILURE);
	}
	command = *(char **)vec_get(pipex->input->cmd, 0);
	if (ft_ispresent(command, '/'))
		ft_handle_absolute(command, shell, &paths);
	else
	{
		ft_check_builtin(shell, command, pipex);
		if (pipex->exec_type == EXTERNAL) 
			ft_handle_relative(pipex->input->cmd, &command, shell, &paths);
	}
	ft_free_splitted_paths(paths);
}
