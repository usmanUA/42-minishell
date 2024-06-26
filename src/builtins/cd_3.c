/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 10:48:29 by mkorpela          #+#    #+#             */
/*   Updated: 2024/06/05 13:24:12 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_oldpwd(t_shell *shell)
{
	t_envp	*oldpwd;
	t_envp	*pwd;

	oldpwd = search_for_envp(shell, "OLDPWD");
	pwd = search_for_envp(shell, "PWD");
	if (oldpwd != NULL)
	{
		free(oldpwd->value);
		if (pwd != NULL && pwd->value != NULL)
		{
			oldpwd->value = ft_strdup(pwd->value);
			if (oldpwd->value == NULL)
			{
				return (malloc_status_failure(shell));
			}
		}
		else
		{
			oldpwd->value = create_empty_string();
			if (oldpwd->value == NULL)
				return (malloc_status_failure(shell));
		}
	}
	return (0);
}

static char	*get_cwd(void)
{
	char	*cwd;
	char	*buffer;

	buffer = NULL;
	cwd = getcwd(buffer, sizeof(buffer));
	return (cwd);
}

int	change_oldpwd_and_pwd(t_shell *shell)
{
	t_envp	*pwd;
	bool	error;

	error = false;
	if (change_oldpwd(shell) == 1)
		error = true;
	pwd = search_for_envp(shell, "PWD");
	if (pwd != NULL)
	{
		free(pwd->value);
		pwd->value = get_cwd();
		if (pwd->value == NULL)
		{
			error = true;
		}
	}
	if (error == true)
		return (1);
	else
		return (0);
}
