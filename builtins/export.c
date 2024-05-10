/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:39:59 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/09 13:42:35 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniwell.h"

bool	check_export_syntax(char *argument)
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
			error_msg_hardcode("export", argument, 5, true);
			// printf("bash: export: `%s': not a valid identifier\n", argument);
			// printf("Error after first character. Should be alphabet (upper or lower), digit or underscore.\n");
			return (true);
		}
		i++;
	}
	return (false);
}

void	if_env_var_exists(t_shell *data, char *command)
{
	t_envp	*node;

	node = search_for_envp(data, command);
	if (node != NULL)
	{
		// printf("env_variable already exists\n");
		//If there is a value after equals sign
		if (ft_strchr(command, '=') != NULL)
		{
			// node->value = NULL;
			free(node->value);
			node->value = get_value_of_env_variable(data, command);
			// printf("env_variable VALUE changed\n");				
			// printf("%s=%s\n", node->key, node->value);
		}
		else
		{
			// printf("env_variable Value NOT changed\n");
			// printf("%s=%s\n", node->key, node->value);
		}
	}
}

void	if_env_var_does_not_exist(t_shell *data, char *command)
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
			// printf("New environmental variable created with NULL-value\n");
			// printf("%s=%s\n", node->key, node->value);				
		}
		else
		{
			node->value = get_value_of_env_variable(data, command);		
			// printf("New environmental variable created with non-NULL-value\n");
			// printf("%s=%s\n", node->key, node->value);
		}
		ft_listadd_back(&(data->env_list), node);
		// printf("%s=%s\n", node->key, node->value);
	}
}

void	print_in_almost_alphabetical_order(t_shell *data, char *j)
{
	t_envp	*temp;
	// char	i;
	
	// i = 'S';
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

int	export_command(t_shell *data, char **command)
{
	int		i;
	char	j;
	bool	error;
	
	// printf("Hello export function\n");
	if (command[1] == NULL)
	{
		// print_in_alphabetical_order(data);
		j = ' ';
		while (j < 127)
		{
			print_in_almost_alphabetical_order(data, &j);
			j++;
		}
		return (0);
	}

	if (command[1] != NULL)
	{
		i = 1;
		error = false;
		while (command[i])
		{
			error = check_export_syntax(command[i]);
			if (error == false)
			{
				if_env_var_exists(data, command[i]);
				if_env_var_does_not_exist(data, command[i]);				
			}
			i++;
		}
	}
	return (0);
}
