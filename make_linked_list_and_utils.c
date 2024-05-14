/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_linked_list_and_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 09:46:32 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/09 14:24:15 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envp	*ft_listlast(t_envp *lst)
{
	while (lst != NULL)
	{
		if (lst->next == NULL)
			break ;
		lst = lst->next;
	}
	return (lst);
}

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

t_envp *create_new_node(t_shell *data)
{
	t_envp	*node;

	node = (t_envp *)malloc(sizeof(t_envp));
	if (node == NULL)
	{
		free_env_list(data);
		exit(1);
	}	
	node->key = NULL;;
	node->value = NULL;
	node->next = NULL;
	return (node);
}

int	make_linked_list_of_envp(t_shell *data, char **envp)
{
	t_envp	*list;
	t_envp	*node;
	int		i;

	list = NULL;
	// if (data->envp == NULL)//If env is set to NULL (f.ex. by running "env -i") [this protects against SEGFAULT]
	// {
	// 	//error msg???
	// 	return (0);//Technically it's ok if envp is set to NULL -> so no error number
	// }
	i = 0;
	while (envp[i])
	{
		node = create_new_node(data);
		node->key = get_name_of_env_variable(data, envp[i]);
		node->value = get_value_of_env_variable(data, envp[i]);
		ft_listadd_back(&list, node);
		i++;
	}
	data->env_list = list;
	return (0);
}

