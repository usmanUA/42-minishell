/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 12:47:43 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/09 14:23:43 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd_command(t_shell *data)
{
	char	*cwd;
	char	*buffer;

	buffer = NULL;
	cwd = getcwd(buffer, sizeof(buffer));
	if (cwd == NULL)
	{
		free_env_list(data);
		exit (errno);
	}
	printf("%s\n", cwd);// DO I have to take dup2 into account?
	free(cwd);
	return (0);
}
