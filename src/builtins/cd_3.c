/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 10:48:29 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/18 10:56:23 by mkorpela         ###   ########.fr       */
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

void	change_oldpwd(t_shell *data, char *directory)
{
	t_envp	*oldpwd;
	t_envp	*pwd;

	oldpwd = search_for_envp(data, "OLDPWD");
	pwd = search_for_envp(data, "PWD");
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
	pwd = search_for_envp(data, "PWD");
	if (pwd != NULL)
	{
		free(pwd->value);
		pwd->value = ft_strdup(directory);
		free(directory);
		if (pwd->value == NULL)
		{
			free_env_list(data);//DEL?
			exit (1);
		}
	}
}
