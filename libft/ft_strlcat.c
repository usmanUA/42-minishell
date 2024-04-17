/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:32:37 by uahmed            #+#    #+#             */
/*   Updated: 2023/10/31 20:06:51 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	d_len;
	size_t	i;
	size_t	s_len;

	s_len = ft_strlen(src);
	if (!size)
		return (s_len);
	d_len = ft_strlen(dst);
	if (size < d_len)
		return (s_len + size);
	s_len += d_len;
	i = -1;
	while (((char *)src)[++i] != '\0' && d_len + 1 < size)
		dst[d_len++] = ((char *)src)[i];
	dst[d_len] = '\0';
	return (s_len);
}
