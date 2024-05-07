/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_remove.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:54:47 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/12 16:54:49 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "vec.h"

int	vec_remove(t_vec *src, size_t index)
{
	if (!src || index > src->len)
		return (0);
	if (index == src->len)
	{
		src->len--;
		return (1);
	}
	ft_memmove(vec_get(src, index), &src->mem[(index + 1) * src->size],
			(src->len - index) * src->size);
	src->len--;
	return (1);
}
