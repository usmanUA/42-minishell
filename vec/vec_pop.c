/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_pop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:54:47 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/12 16:54:49 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "vec.h"

int vec_pop(void *dst, t_vec *src)
{
    if (!dst || !src)
	return (-1);
    memcpy(dst, vec_get(src, src->len-1), src->size);
    //NOTE: POP DOES NOT REMOVE THE LAST ELEMENT FROM MEMORY
    //NOTE: INSTEAD ONLY DECREMENTS LEN
    src->len--;
    return (1);
}
