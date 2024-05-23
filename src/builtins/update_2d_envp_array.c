/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_2d_envp_array.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 09:58:54 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/23 10:12:12 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	number_of_envs_with_values(t_shell *shell)
{
	t_envp	*temp;
	int		i;

	temp = shell->env_list;
	i = 0;
	while (temp && temp->key)
	{
		if (temp->value != NULL)
		{
			i++;
		}
		temp = temp->next;
	}
	return (i);
}

static char	*copy_envs_from_linked_list(t_envp *node)
{
	char	*partial_env;
	char	*full_env;

	partial_env = ft_strjoin(node->key, "=");
	if (partial_env == NULL)
	{
		return (NULL);
	}
	full_env = ft_strjoin(partial_env, node->value);
	free(partial_env);
	if (full_env == NULL)
	{
		return (NULL);
	}
	return (full_env);
}

int	update_2d_env_array(t_shell *shell)
{
	char	**new_env;
	int		i;
	t_envp	*node;

	free_env_array(shell);
	new_env = malloc(sizeof(char *) * (number_of_envs_with_values(shell) + 1));
	if (new_env == NULL)
		return (1);
	i = 0;
	node = shell->env_list;
	while (node && node->key)
	{
		if (node->value != NULL)
		{
			new_env[i] = copy_envs_from_linked_list(node);
			if (new_env[i] == NULL)
				return (free_failed_2d_array(shell, new_env, i));
			i++;
		}
		node = node->next;
	}
	new_env[i] = NULL;
	shell->envp = new_env;
	return (0);
}
