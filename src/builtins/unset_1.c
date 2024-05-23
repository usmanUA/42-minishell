/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:25:01 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/22 14:59:07 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	return_index_of_node_above_unset_node(t_shell *shell, t_envp *unset_node)
{
	t_envp	*temp;
	int		i;

	temp = shell->env_list;
	i = 0;
	while (temp->next != unset_node)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

bool	check_unset_syntax(char *argument)
{
	int	i;

	i = 0;
	if (ft_isalpha(argument[0]) == 0 && argument[0] != '_')
	{
		error_msg("unset", argument, 5, true);
		return (true);
	}
	i++;
	while (argument[i])
	{
		if (ft_isalnum(argument[i]) == 0 && argument[i] != '_')
		{
			error_msg("unset", argument, 5, true);
			return (true);
		}
		i++;
	}
	return (false);
}

int	unset_command(t_shell *shell, char **command)
{
	int		i;
	bool	arg_error;
	bool	error_flag;

	if (command[1] == NULL)
		return (0);
	i = 1;
	error_flag = false;
	while (command[i])
	{
		arg_error = false;
		arg_error = check_unset_syntax(command[i]);
		if (arg_error == true)
			error_flag = true;
		if (arg_error == false)
		{
			delete_environmental_variable(shell, command[i]);
		}
		i++;
	}
	if (update_2d_env_array(shell) == 1)
		return (1);
	return (ft_return_value(error_flag));
}
