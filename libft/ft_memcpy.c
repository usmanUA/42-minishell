/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 14:44:08 by mkorpela          #+#    #+#             */
/*   Updated: 2023/11/19 21:46:34 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*ptr_dst;
	char	*ptr_src;

	ptr_dst = (char *)dst;
	ptr_src = (char *)src;
	i = 0;
	while (i < n && (src || dst))
	{
		ptr_dst[i] = ptr_src[i];
		i++;
	}
	return (dst);
}
