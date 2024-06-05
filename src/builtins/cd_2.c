/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 10:28:44 by mkorpela          #+#    #+#             */
/*   Updated: 2024/06/05 13:16:20 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*make_relative_path(t_shell *shell, char *command, char *cwd)
{
	char	*cwd_with_slash;
	char	*directory;

	cwd_with_slash = ft_strjoin(cwd, "/");
	free(cwd);
	if (cwd_with_slash == NULL)
	{
		shell->malloc_status = FAILURE;
		return (NULL);
	}
	directory = ft_strjoin(cwd_with_slash, command);
	free(cwd_with_slash);
	if (directory == NULL)
	{
		shell->malloc_status = FAILURE;
		return (NULL);
	}
	return (directory);
}

void	cleanup_after_getcwd_failed(t_shell *shell)
{
	t_envp	*pwd_node;

	error_msg_2(1);
	change_oldpwd(shell);
	pwd_node = search_for_envp(shell, "PWD");
	free(pwd_node->value);
	pwd_node->value = create_empty_string();
}

char	*relative_path(t_shell *shell, char *command)
{
	char	*cwd;
	char	*buffer;
	char	*directory;

	buffer = NULL;
	cwd = getcwd(buffer, sizeof(buffer));
	if (cwd == NULL)
	{
		cleanup_after_getcwd_failed(shell);
		return (NULL);
	}
	directory = make_relative_path(shell, command, cwd);
	return (directory);
}
