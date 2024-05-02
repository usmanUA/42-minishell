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
#include "miniwell.h"
#include "vec/vec.h"
#include <stdio.h>
#include <stdlib.h>

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

void	ft_handle_absolute(char *command, t_vec *info)
{
	int	fd;
	int	cmd_flag;

	fd = -2;
	cmd_flag = RED;
	if (!access(command, F_OK))
	{
		fd = open(command, O_DIRECTORY);
		if (fd != -1)
		{
			close(fd);
			ft_cmd_error(command, 2, 1); // NOTE: is a directory
			// TODO: return relevant exit status 126
		}
		else if (access(command, X_OK) == -1)
		{
			ft_cmd_error(command, 0, 1);
			// TODO: command no permission, 
			// return 126 exit status
		}
		else
			cmd_flag = GREEN;
			// TODO: command is OK, continue executing
	}
	// NOTE: if absolute command (given) does not exist
	if (cmd_flag == RED)
		ft_cmd_error(command, 0, 0); // TODO: define MACROS for const. values
	// TODO: return exit status 127
	if (!vec_push(info, &cmd_flag))
		return; // NOTE: malloc fail
}

void	ft_handle_relative(t_vec *cmd, t_vec *info, char **paths)
{
	char	*cmd_path;
	char	*command;
	int	i;
	int	cmd_flag;	

	i = -1;
	cmd_flag = RED;
	command = *(char **)vec_get(cmd, 0);
	while (paths[++i])
	{
		cmd_path = ft_join_path(paths[i], command);
		if (!access(cmd_path, F_OK))
		{
			if (access(cmd_path, X_OK) == -1)
			{
				ft_cmd_error(command, 0, 1);
				// TODO: return exit status 126	
			}
			else
			{
				// TODO: look for handling ft_memmove and ft_memcpy failures 
				// WARN: make sure you're doing correctly
				vec_insert(cmd, &cmd_path, 0); // NOTE: make sure the address of cmd_path goes along
				vec_remove(cmd, 1); // NOTE: overwrites the address of command (overwritten address is the pointer to the address of command) 
				free(command); // NOTE: free the command (command is the pointer to the first char of malloced string)
				cmd_flag = GREEN;
			}	
			break;
		}
		free(cmd_path);
	}
	// NOTE: if it gets here it means command does not exits
	// TODO: return exit status 127
	if (cmd_flag == RED)
		ft_cmd_error(command, 1, 1);
	if (!vec_push(info, &cmd_flag))
		return; // NOTE: malloc fail
}

int	ft_validate_commands(t_input *input, t_vec *info, char **envp)
{
	char **paths;
	char *command;
	
	paths = ft_split(ft_give_path(envp), ':');
	if (!paths)
		return (0); // TODO: check malloc error handling
	command = *(char **)vec_get(input->cmd, 0);
	if (ft_ispresent(command, '/'))
		ft_handle_absolute(command, info);
	else
		ft_handle_relative(input->cmd, info, paths);
	return (1);
}

