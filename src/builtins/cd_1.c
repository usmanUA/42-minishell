/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:37:56 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/18 10:55:29 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*absolute_path(t_shell *data, char *command)
{
	char *directory;

	// printf("ABS - command = cd %s\n", command);
	directory = ft_strdup(command);
	if (directory == NULL)
	{
		free_env_list(data);//Change this?
		exit (1);
	}
	return (directory);
}

char	*only_cd(t_shell *data)
{
	char *directory;
	t_envp	*node;	

	// printf("0 - command = cd\n");
	node = search_for_envp(data, "HOME");
	if (node == NULL || node->value == NULL)
	{
		error_msg_hardcode("cd", NULL, 4, false);
		// printf("bash: cd: HOME not set\n");
		return (NULL);
	}
	directory = ft_strdup(node->value);
	if (directory == NULL)
	{
		free_env_list(data);//Change this?
		exit (1);
	}
	return (directory);
}

int	 change_to_directory(t_shell *data, char *directory, char *command)
{
	char	*home_error;
	t_envp	*home_node;
	
	home_node = search_for_envp(data, "HOME");
	if (chdir(directory) == -1)
	{
		if (command == NULL)
		{
			error_msg_hardcode("cd", home_node->value, 0, false);
			return (1);
		}
		else
		{
			error_msg_hardcode("cd", command, 0, false);
			return (1);
		}
	}
	return (0);
}

char	*find_directory(t_shell *data, char *command)
{
	char	*directory;

	if (command == NULL)
	{
		directory = only_cd(data);
	}
	else if (command[0] == '/')
	{
		directory = absolute_path(data, command);
	}
	else
	{
		directory = relative_path(data, command);
	}
	return (directory);
}

int	cd_command(t_shell *data, char **command)
{
	char	*directory;

	directory = find_directory(data, command[1]);
	if (directory == NULL)
	{
		return (1);
	}
	if (change_to_directory(data, directory, command[1]) == 1)
	{
		return (1);
	}
	change_oldpwd_and_pwd(data, directory);
	return (0);
}
