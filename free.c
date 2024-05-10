/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:46:09 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/09 14:20:55 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_list(t_shell *data)
{
	t_envp	*prev_node;
	t_envp	*curr_node;
	
	curr_node = data->env_list;
	while (curr_node)
	{
		prev_node = curr_node;
		curr_node = curr_node->next;
		free(prev_node->key);
		free(prev_node->value);
		free(prev_node);
	}
	data->env_list = NULL;
}

void	free_all(t_shell *data)
{
	free_env_list(data);
}
