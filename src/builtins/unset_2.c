/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:55:24 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/22 10:17:17 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_envp	*delete_orphan_node(t_shell *shell)
{
	t_envp	*temp;

	temp = shell->env_list;
	free(temp->key);
	free(temp->value);
	free(temp);
	return (NULL);
}

static t_envp	*delete_top_of_list(t_shell *shell)
{
	t_envp	*temp;
	t_envp	*new_top_of_list;

	temp = shell->env_list;
	free(temp->key);
	free(temp->value);
	new_top_of_list = temp->next;
	free(temp);
	return (new_top_of_list);
}

static void	delete_bottom_of_list(t_shell *shell, t_envp *unset_node)
{
	t_envp	*new_bottom_of_list;
	int		index;
	int		i;

	index = return_index_of_node_above_unset_node(shell, unset_node);
	i = 0;
	new_bottom_of_list = shell->env_list;
	while (index > i)
	{
		new_bottom_of_list = new_bottom_of_list->next;
		i++;
	}
	free(new_bottom_of_list->next->key);
	free(new_bottom_of_list->next->value);
	free(new_bottom_of_list->next);
	new_bottom_of_list->next = NULL;
}

static void	delete_from_middle_of_list(t_shell *shell, t_envp *unset_node)
{
	t_envp	*node;
	t_envp	*temp;
	int		index;
	int		i;

	index = return_index_of_node_above_unset_node(shell, unset_node);
	i = 0;
	node = shell->env_list;
	while (index > i)
	{
		node = node->next;
		i++;
	}
	free(node->next->key);
	free(node->next->value);
	temp = node->next->next;
	free(node->next);
	node->next = temp;
}

void	delete_environmental_variable(t_shell *shell, char *command)
{
	t_envp	*node;

	node = search_for_envp(shell, command);
	if (node == NULL)
	{
		return ;
	}
	if (node == shell->env_list && node->next == NULL)
	{
		shell->env_list = delete_orphan_node(shell);
	}
	else if (node == shell->env_list)
	{
		shell->env_list = delete_top_of_list(shell);
	}
	else if (node->next == NULL)
	{
		delete_bottom_of_list(shell, node);
	}
	else
	{
		delete_from_middle_of_list(shell, node);
	}
}
