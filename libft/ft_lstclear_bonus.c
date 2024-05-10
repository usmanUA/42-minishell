/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 17:31:10 by mkorpela          #+#    #+#             */
/*   Updated: 2024/02/05 11:17:44 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h" 

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*prev_node;
	t_list	*curr_node;

	prev_node = *lst;
	curr_node = NULL;
	while (prev_node != NULL)
	{
		del(prev_node->content);
		curr_node = prev_node->next;
		free(prev_node);
		prev_node = curr_node;
	}
	*lst = NULL;
}
