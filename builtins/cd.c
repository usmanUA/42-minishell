/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:37:56 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/16 11:14:43 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniwell.h"

char	*create_empty_string(t_shell *data, char *directory)
{
	char	*string;

	string = (char *)malloc(sizeof(char) * 1);
	if (string == NULL)
	{
		free_env_list(data);
		free(directory);
		exit (1);
	}
	string[0] = '\0';
	return (string);
}

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

void	change_oldpwd(t_shell *data, char *directory)
{
	t_envp	*oldpwd;
	t_envp	*pwd;

	oldpwd = search_for_envp(data, "OLDPWD");//What if it finds NULL?
	pwd = search_for_envp(data, "PWD");//What if it finds NULL?	
	if (oldpwd != NULL)
	{
		free(oldpwd->value);
		if (pwd != NULL)
		{
			oldpwd->value = ft_strdup(pwd->value);
			if (oldpwd->value == NULL)
			{
				free(directory);
				free_env_list(data);
				exit (1);
			}
		}
		else
		{
			oldpwd->value = create_empty_string(data, directory);
		}
	}
}

void	change_oldpwd_and_pwd(t_shell *data, char *directory)
{
	t_envp	*pwd;

	change_oldpwd(data, directory);
	pwd = search_for_envp(data, "PWD");//What if it finds NULL?	
	if (pwd != NULL)
	{
		free(pwd->value);
		pwd->value = ft_strdup(directory);
		free(directory);
		if (pwd->value == NULL)
		{
			free_env_list(data);
			exit (1);
		}
	}
}

int	 change_to_directory(t_shell *data, char *directory, char *command)
{
	char	*home_error;
	t_envp	*home_node;
	
	home_node = search_for_envp(data, "HOME");
	if (chdir(directory) == -1)
	{
		// printf("errno: %d\n", errno);
		if (command == NULL)	// Figure this edge case later... It's obscure and not super important.
		{
			// printf("errno: %d\n", errno);
			error_msg_hardcode("cd", home_node->value, 0, false);
			// // home_error = get_value_of_env_variable(data, command);
			
			// home_error = malloc(sizeof(char) * (f_strlen(home_node->value) + 1));
			// if (home_error == NULL)
			// {
			// 	//free all
			// 	exit (1);
			// }
			// home_error = string_n_copy(home_error, home_node->value, f_strlen(home_node->value));
			// error_msg_hardcode("cd", home_error, 0);
			// //perror
			// printf("bash: cd: %s: No such file or directory\n", home_error);// - something like this
			// free(home_error);//free(home_error);
			return (1);
		}
		else
		{
			error_msg_hardcode("cd", command, 0, false);//te
			// if (errno == ENOENT)
			// {
			// 	printf("bash: cd: %s: No such file or directory\n", command);
			// 	return (1);
			// }
			// if (errno == ENOTDIR)
			// {
			// 	printf("bash: cd: %s: Not a directory\n", command);
			// 	return (1);
			// }
			// if (errno == EACCES)
			// {
			// 	printf("bash: cd: %s/: Permission denied\n", command);
			// 	return (1);
			// }
			return (1);//test this - or do I return another number?
		}
	}
	return (0);
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
		free_env_list(data);
		exit (1);
	}
	return (directory);
}

char	*absolute_path(t_shell *data, char *command)
{
	char *directory;

	// printf("ABS - command = cd %s\n", command);
	directory = ft_strdup(command);
	if (directory == NULL)
	{
		free_env_list(data);
		exit (1);
	}
	return (directory);
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
		exit(1);
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
			free_env_list(data);
			exit(1);
		}
		change_oldpwd_and_pwd(data, empty_string);
		// pwd = search_for_envp(data, "PWD");
		// if (pwd != NULL && pwd->value != NULL)
		// {
		// 	free(pwd->value);
		// 	pwd->value = create_empty_str(data);
		// }
		//PWD = NULL / "";
		return (NULL);
	}
	// printf("4 - command = cd %s\n", command);
	directory = make_relative_path(data, command, cwd);	
	return (directory);
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
	
	// pwd_command(data);//This is to print the current directory ( -- JUST FOR CONFIRMATION -- I promise)
	return (0);
}
