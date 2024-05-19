/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 10:06:18 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/18 10:23:16 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	if_env_var_does_not_exist(t_shell *data, char *command)
{
	t_envp	*node;

	node = search_for_envp(data, command);
	if (node == NULL)
	{
		node = create_new_node(data);
		node->key = get_name_of_env_variable(data, command);
		if (ft_strchr(command, '=') == NULL)
		{
			node->value = NULL;
		}
		else
		{
			node->value = get_value_of_env_variable(data, command);
		}
		ft_listadd_back(&(data->env_list), node);
	}
}

static bool	append_to_value(t_shell *data, char *command)
{
	bool	append;
	char	*value;

	append = false;
	value = ft_strchr(command, '=');
	value--;
	if (*value == '+')
	{
		append = true;
	}
	return (append);
}

static void	if_env_var_exists(t_shell *data, char *command)
{
	t_envp	*node;
	char	*value_append;
	char	*new_value;

	node = search_for_envp(data, command);
	if (node != NULL)
	{
		if (ft_strchr(command, '=') != NULL)
		{
			if (append_to_value(data, command) == false)
			{
				free(node->value);
				node->value = get_value_of_env_variable(data, command);
			}
			else
			{
				value_append = get_value_of_env_variable(data, command);
				new_value = ft_strjoin(node->value, value_append);
				free(value_append);
				free(node->value);
				node->value = new_value;
			}
		}
	}
}

static bool	check_export_syntax(char *argument)
{
	int	i;

	i = 0;
	if (ft_isalpha(argument[0]) == 0 && argument[0] != '_')
	{
		error_msg_hardcode("export", argument, 5, true);
		// printf("bash: export: `%s': not a valid identifier\n", argument);
		// printf("Error in First Character - it should be either an Uppercase, Lowercase Letter or Underscore\n");
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
			error_msg_hardcode("export", argument, 5, true);
			// printf("bash: export: `%s': not a valid identifier\n", argument);
			// printf("Error after first character. Should be alphabet (upper or lower), digit or underscore.\n");
			return (true);
		}
		i++;
	}
	return (false);
}

bool	export_with_arguments(t_shell *data, char **command, bool error_flag)
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
				if_env_var_exists(data, command[i]);
				if_env_var_does_not_exist(data, command[i]);
				update_2d_envp_array(data, command[i]);
			}
			i++;
		}
	}
	return (error_flag);
}
