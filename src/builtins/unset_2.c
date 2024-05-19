/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:55:24 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/17 14:16:53 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniwell.h"

static t_envp	*delete_orphan_node(t_shell *data)
{
	t_envp	*temp;

	temp = data->env_list;
	free(temp->key);
	free(temp->value);
	free(temp);
	return (NULL);
}

static t_envp	*delete_top_of_list(t_shell *data)
{
	t_envp	*temp;
	t_envp	*new_top_of_list;

	temp = data->env_list;
	free(temp->key);
	free(temp->value);
	new_top_of_list = temp->next;
	free(temp);
	return (new_top_of_list);
}

static void	delete_bottom_of_list(t_shell *data, t_envp *unset_node)
{
	t_envp	*new_bottom_of_list;

	new_bottom_of_list = go_to_node_above_unset_node(data, unset_node);
	free(new_bottom_of_list->next->key);
	free(new_bottom_of_list->next->value);
	free(new_bottom_of_list->next);
	new_bottom_of_list->next = NULL;
}

static void	delete_from_middle_of_list(t_shell *data, t_envp *unset_node)
{
	t_envp	*node;
	t_envp	*temp;

	node = go_to_node_above_unset_node(data, unset_node);
	free(node->next->key);
	free(node->next->value);
	temp = node->next->next;
	free(node->next);
	node->next = temp;
}

void	delete_environmental_variable(t_shell *data, char *command)
{
	t_envp	*node;

	node = search_for_envp(data, command);
	if (node == NULL)
	{
		return ;
	}
	if (node == data->env_list && node->next == NULL)
	{
		data->env_list = delete_orphan_node(data);
	}
	else if (node == data->env_list)
	{
		data->env_list = delete_top_of_list(data);
	}
	else if (node->next == NULL)
	{
		delete_bottom_of_list(data, node);
	}
	else
	{
		delete_from_middle_of_list(data, node);
	}
}
