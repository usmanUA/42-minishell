/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:39:59 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/18 10:21:24 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniwell.h"

void	print_in_alphabetical_order(t_shell *data, char *j)
{
	t_envp	*temp;

	temp = data->env_list;
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

int	just_export(t_shell *data, char **command)
{
	char	j;

	j = ' ';
	while (j < 127)
	{
		print_in_alphabetical_order(data, &j);
		j++;
	}
	return (0);
}

int	export_command(t_shell *data, char **command)
{
	int		i;
	bool	error_flag;

	if (command[1] == NULL)
	{
		return (just_export(data, command));
	}
	error_flag = false;
	if (command[1] != NULL)
	{
		error_flag = export_with_arguments(data, command, error_flag);
	}
	return (ft_return_value(error_flag));
}
