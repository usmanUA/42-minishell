/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:25:01 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/09 13:37:39 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniwell.h"

t_envp	*delete_orphan_node(t_shell *data)
{
	t_envp	*temp;

	temp = data->env_list;
	// printf("%s=%s\n", temp->key, temp->value);
	free(temp->key);
	free(temp->value);
	free(temp);
	return (NULL);
}

t_envp	*delete_top_of_list(t_shell *data)
{
	t_envp	*temp;
	t_envp	*new_top_of_list;

	temp = data->env_list;
	// printf("%s=%s\n", temp->key, temp->value);
	free(temp->key);
	free(temp->value);
	new_top_of_list = temp->next;
	free(temp);
	return (new_top_of_list);
}
t_envp	*go_to_node_above_unset_node(t_shell *data, t_envp *unset_node)
{
	t_envp	*temp;
	
	temp = data->env_list;
	while(temp->next != unset_node)
	{
		temp = temp->next;
	}
	return (temp);
}

void	delete_bottom_of_list(t_shell *data, t_envp *unset_node)
{
	t_envp	*new_bottom_of_list;

	new_bottom_of_list = go_to_node_above_unset_node(data, unset_node);
	// printf("%s=%s\n",new_bottom_of_list->next->key, new_bottom_of_list->next->value);
	free(new_bottom_of_list->next->key);
	free(new_bottom_of_list->next->value);
	free(new_bottom_of_list->next);
	new_bottom_of_list->next = NULL;
}

void	delete_from_middle_of_list(t_shell *data, t_envp *unset_node)
{
	t_envp *node;
	t_envp *temp;
	
	node = go_to_node_above_unset_node(data, unset_node);
	// printf("%s=%s\n", node->next->key, node->next->value);
	free(node->next->key);
	free(node->next->value);
	temp = node->next->next;
	free(node->next);
	node->next = temp;
}

void	delete_environmental_variable(t_shell *data, char *command)
{
	t_envp	*node;	
	
	node = search_for_envp(data, command);//FIND matching ENVP
	if (node == NULL)
	{
		// printf("No environmental variable found - no variable deleted\n");
		return ;
	}
	if (node == data->env_list && node->next == NULL)
	{
		data->env_list = delete_orphan_node(data);
		// printf("Deleted orphan node\n");
	}
	else if (node == data->env_list)//TOP of stack
	{
		data->env_list = delete_top_of_list(data);
		// printf("Deleted node from TOP\n");
	}
	else if (node->next == NULL)
	{
		delete_bottom_of_list(data, node);
		// printf("Deleted node from BOTTOM\n");		
	}
	else
	{
		delete_from_middle_of_list(data, node);
		// printf("Deleted node from MIDDLE of list\n");	
	}
}

bool	check_unset_syntax(char *argument)
{
	int	i;

	i = 0;
	if (ft_isalpha(argument[0]) == 0 && argument[0] != '_')
	{
		error_msg_hardcode("unset", argument, 5, true);
		// printf("bash: unset: `%s': not a valid identifier\n", argument);
		// printf("Error in First Character - it should be either an Uppercase, Lowercase Letter or Underscore\n");
		return (true);
	}
	i++;
	while (argument[i])
	{
		if (ft_isalnum(argument[i]) == 0 && argument[i] != '_')
		{
			error_msg_hardcode("unset", argument, 5, true);
			// printf("bash: unset: `%s': not a valid identifier\n", argument);
			// printf("Error after first character. Should be alphabet (upper or lower), digit or underscore.\n");
			return (true);
		}
		i++;
	}
	return (false);
}

int	unset_command(t_shell *data, char **command)
{
	int		i;
	bool	error;

	if (command[1] == NULL)
	{
		return (0);
	}
	i = 1;	
	while (command[i])
	{
		error = false;
		error = check_unset_syntax(command[i]);//check for error value
		if (error == true)
		{
			delete_environmental_variable(data, command[i]);
		}
		i++;
	}
	return (0);
}
