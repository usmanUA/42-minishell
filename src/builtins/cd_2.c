/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 10:28:44 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/18 10:55:31 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_empty_str(t_shell *data)
{
	char	*string;

	string = (char *)malloc(sizeof(char) * 1);
	if (string == NULL)
	{
		free_env_list(data);
		exit (1);
	}
	string[0] = '\0';
	return (string);
}

char	*make_relative_path(t_shell *data, char *command, char *cwd)
{
	char	*cwd_with_slash;
	char	*directory;

	cwd_with_slash = ft_strjoin(cwd, "/");
	// printf("cwd_with_slash: %s\n", cwd_with_slash);
	if (cwd_with_slash == NULL)
	{
		free(cwd);
		free_env_list(data);
		exit (1);
	}
	free(cwd);
	directory = ft_strjoin(cwd_with_slash, command);
	// printf("new_directory: %s\n", directory);
	if (directory == NULL)
	{
		free(cwd_with_slash);
		free_env_list(data);
		exit (1);
	}
	free(cwd_with_slash);
	return (directory);
}

char	*relative_path(t_shell *data, char *command)
{
	char	*cwd;
	char	*buffer;
	char	*directory;
	char	*empty_string;
	
	buffer = NULL;
	cwd = getcwd(buffer, sizeof(buffer));
	if (cwd == NULL)
	{
		error_msg_2(1);
		empty_string = create_empty_str(data);
		if (empty_string == NULL)
		{
			free_env_list(data);//Change this?
			exit(1);
		}
		change_oldpwd_and_pwd(data, empty_string);
		return (NULL);
	}
	directory = make_relative_path(data, command, cwd);	
	return (directory);
}
