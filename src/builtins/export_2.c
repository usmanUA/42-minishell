/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 10:06:18 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/22 11:18:36 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_key_value(t_shell *shell, char *command, t_envp *node)
{
	(void)shell;
	if (ft_strchr(command, '=') == NULL)
	{
		node->value = NULL;
		return (0);
	}
	else
	{
		node->value = get_value_of_env_variable(command);
		if (node->value == NULL)
		{
			free(node->value);
			free(node);
			return (1);
		}
		return (0);
	}
}

static int	if_env_var_does_not_exist(t_shell *shell, char *command)
{
	t_envp	*node;

	node = search_for_envp(shell, command);
	if (node == NULL)
	{
		node = create_new_node();
		if (node == NULL)
		{
			return (1);
		}
		node->key = get_name_of_env_variable(command);
		if (node->key == NULL)
		{
			free(node);
			return (1);
		}
		if (set_key_value(shell, command, node) == 1)
			return (1);
		ft_listadd_back(&(shell->env_list), node);
	}
	return (0);
}

static bool	check_export_syntax(char *argument)
{
	int	i;

	i = 0;
	if (ft_isalpha(argument[0]) == 0 && argument[0] != '_')
	{
		error_msg("export", argument, 5, true);
		return (true);
	}
	i++;
	while (argument[i] && argument[i] != '=')
	{
		if (ft_isalnum(argument[i]) == 0 && argument[i] != '_')
		{
			if ((argument[i]) == '+' && argument[i + 1] == '=')
			{
				break ;
			}
			error_msg("export", argument, 5, true);
			return (true);
		}
		i++;
	}
	return (false);
}

bool	export_with_arguments(t_shell *shell, char **command, bool error_flag)
{
	bool	arg_error;
	int		i;

	arg_error = false;
	if (command[1] != NULL)
	{
		i = 1;
		error_flag = false;
		while (command[i])
		{
			arg_error = false;
			arg_error = check_export_syntax(command[i]);
			if (arg_error == true)
				error_flag = true;
			if (arg_error == false)
			{
				if (if_env_var_exists(shell, command[i]) == 1)
					error_flag = true;
				if (if_env_var_does_not_exist(shell, command[i]) == 1)
					error_flag = true;
			}
			i++;
		}
	}
	return (error_flag);
}
