/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 14:27:59 by mkorpela          #+#    #+#             */
/*   Updated: 2024/02/01 15:25:36 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	srclen;
	size_t	dstlen;

	i = 0;
	if (dstsize == 0)
	{
		return (ft_strlen(src));
	}
	srclen = ft_strlen(src);
	dstlen = ft_strlen(dst);
	if (dstsize - 1 < dstlen)
	{
		return (srclen + dstsize);
	}
	else
	{
		while (i < dstsize - dstlen - 1 && src[i] != '\0')
		{
			dst[dstlen + i] = src[i];
			i++;
		}
		dst[i + dstlen] = '\0';
	}
	return (srclen + dstlen);
}
