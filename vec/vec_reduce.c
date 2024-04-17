/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_reduce.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:54:47 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/12 16:54:49 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "vec.h"

int vec_reduce(void *acc, t_vec *src, void (*f) (void *, void *))
{
    int ind;

    ind = -1;
    if (!acc || !src || !src->mem)
	return (-1);
    while(++ind < src->len)
	(f)(acc, &src->mem[ind * src->size]);
    return (1);
}
