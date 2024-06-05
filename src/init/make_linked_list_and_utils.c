/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_linked_list_and_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 09:46:32 by mkorpela          #+#    #+#             */
/*   Updated: 2024/06/05 14:38:24 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_status;

void	ft_listadd_back(t_envp **lst, t_envp *new)
{
	t_envp	*last_node;

	if (*lst)
	{
		last_node = ft_listlast(*lst);
		last_node->next = new;
	}
	else
	{
		*lst = new;
	}
}

t_envp	*create_new_node(void)
{
	t_envp	*node;

	node = (t_envp *)malloc(sizeof(t_envp));
	if (node == NULL)
	{
		return (NULL);
	}
	node->key = NULL;
	node->value = NULL;
	node->next = NULL;
	return (node);
}

int	create_node_and_add_to_list(t_shell *shell, char *envp, t_envp **list)
{
	t_envp	*node;

	node = create_new_node();
	if (node == NULL)
	{
		return (1);
	}
	node->key = get_name_of_env_variable(envp);
	if (node->key == NULL)
	{
		free(node);
		return (1);
	}
	node->value = get_value_of_env_var(shell, envp);
	ft_listadd_back(list, node);
	return (0);
}

static void	make_linked_list_of_envp(t_shell *shell, char **envp)
{
	t_envp	*list;
	int		i;

	list = NULL;
	i = 0;
	while (envp[i])
	{
		if (create_node_and_add_to_list(shell, envp[i], &list) == 1)
			break ;
		i++;
	}
	shell->env_list = list;
	return ;
}

void	allocate_all_envps(t_shell *shell, char **envp, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	shell->status = GREEN;
	g_signal_status = 0;
	shell->envp = malloc_envp(shell, envp);
	increment_2d_shell_level(shell, shell->envp);
	make_linked_list_of_envp(shell, shell->envp);
}
