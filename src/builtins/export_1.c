/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:39:59 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/22 10:50:14 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_in_alphabetical_order(t_shell *shell, char *j)
{
	t_envp	*temp;

	temp = shell->env_list;
	while (temp)
	{
		if (temp->key[0] == *j)
		{
			printf("declare -x %s", temp->key);
			if (temp->value != NULL)
			{
				printf("=\"%s\"", temp->value);
			}
			printf("\n");
		}
		temp = temp->next;
	}
}

static int	just_export(t_shell *shell)
{
	char	j;

	j = ' ';
	while (j < 127)
	{
		print_in_alphabetical_order(shell, &j);
		j++;
	}
	return (0);
}

int	export_command(t_shell *shell, char **command)
{
	bool	error_flag;

	if (command[1] == NULL)
	{
		return (just_export(shell));
	}
	error_flag = false;
	if (command[1] != NULL)
	{
		error_flag = export_with_arguments(shell, command, error_flag);
	}
	if (update_2d_env_array(shell) == 1)
	{
		return (1);
	}
	return (ft_return_value(error_flag));
}
