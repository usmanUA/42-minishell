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

static void	memswap(unsigned char *x, unsigned char *y, size_t bytes)
{
	int	ind;

	if (!x || !y)
		return ;
	ind = -1;
	while (++ind < (int)bytes)
	{
		if (&x[ind] == &y[ind])
			continue ;
		x[ind] ^= y[ind];
		y[ind] ^= x[ind];
		x[ind] ^= y[ind];
	}
}

void	vec_sort(t_vec *src, int (*f)(void *, void *))
{
	int	ind;

	ind = -1;
	if (!src || !src->mem || !f)
		return ;
	while (++ind < (int)src->len - 1)
	{
		if ((f)(&src->mem[ind * src->size], &src->mem[(ind + 1)
				* src->size]) > 0)
		{
			memswap(&src->mem[ind * src->size], &src->mem[(ind + 1)
					* src->size], src->size);
			ind = -1;
		}
	}
}
