/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 12:38:17 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/22 10:15:10 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env_list(t_shell *shell)
{
	t_envp	*temp;

	temp = shell->env_list;
	while (temp && temp->key)
	{
		if (temp->value != NULL)
		{
			printf("%s", temp->key);
			printf("=");
			printf("%s\n", temp->value);
		}
		temp = temp->next;
	}
}

int	env_command(t_shell *shell, char **command)
{
	if (command[1] != NULL)
	{
		error_msg("env", command[1], 3, false);
		return (1);
	}
	print_env_list(shell);
	return (0);
}
