/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 08:32:09 by mkorpela          #+#    #+#             */
/*   Updated: 2023/11/19 21:46:39 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;
	char	*ptr_dst;
	char	*ptr_src;

	ptr_src = (char *)src;
	ptr_dst = (char *)dst;
	if (len == 0)
		return (dst);
	i = 0;
	if (dst < src)
	{
		ft_memcpy(dst, src, len);
	}
	if (dst > src)
	{
		i = len - 1;
		while (i > 0)
		{
			ptr_dst[i] = ptr_src[i];
			i--;
		}
		ptr_dst[i] = ptr_src[i];
	}
	return (dst);
}
