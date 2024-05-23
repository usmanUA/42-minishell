/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:37:56 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/22 10:38:31 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*absolute_path(char *command)
{
	char	*directory;

	directory = ft_strdup(command);
	if (directory == NULL)
	{
		return (NULL);
	}
	return (directory);
}

char	*only_cd(t_shell *shell)
{
	char	*directory;
	t_envp	*node;	

	node = search_for_envp(shell, "HOME");
	if (node == NULL || node->value == NULL)
	{
		error_msg("cd", NULL, 4, false);
		return (NULL);
	}
	directory = ft_strdup(node->value);
	if (directory == NULL)
	{
		return (NULL);
	}
	return (directory);
}

int	change_to_directory(t_shell *shell, char *directory, char *command)
{
	t_envp	*home_node;

	if (chdir(directory) == -1)
	{
		free(directory);
		home_node = search_for_envp(shell, "HOME");
		if (command == NULL)
		{
			error_msg("cd", home_node->value, 0, false);
			return (1);
		}
		else
		{
			error_msg("cd", command, 0, false);
			return (1);
		}
	}
	free(directory);
	return (0);
}

char	*find_directory(t_shell *shell, char *command)
{
	char	*directory;

	if (command == NULL)
	{
		directory = only_cd(shell);
	}
	else if (command[0] == '/')
	{
		directory = absolute_path(command);
	}
	else
	{
		directory = relative_path(shell, command);
	}
	return (directory);
}

int	cd_command(t_shell *shell, char **command)
{
	char	*directory;

	directory = find_directory(shell, command[1]);
	if (directory == NULL)
	{
		return (1);
	}
	if (change_to_directory(shell, directory, command[1]) == 1)
	{
		return (1);
	}
	if (change_oldpwd_and_pwd(shell) == 1)
	{
		return (1);
	}
	if (update_2d_env_array(shell) == 1)
	{
		return (1);
	}
	return (0);
}
