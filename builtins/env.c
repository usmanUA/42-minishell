/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 12:38:17 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/09 13:28:17 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniwell.h"

void	print_env_list(t_shell *data)
{
	t_envp	*temp;

	temp = data->env_list;
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

int	env_command(t_shell *data, char **command)
{
	printf("inside env call: %s\n", command[0]);
	printf("inside env call: %s\n", command[1]);
	if (command[1] != NULL)//perhaps just use 
	{
		error_msg_hardcode("env", command[1], 3, false);
		// printf("Write env - just env. No options, arguments or other shenanigans.\n");
		return (1);
	}
	print_env_list(data);
	return (0);
}
