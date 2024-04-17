/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:25:30 by uahmed            #+#    #+#             */
/*   Updated: 2023/10/31 20:04:19 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcpy(char *restrict dst, const char *restrict src, size_t size)
{
	size_t	ind;

	ind = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (src[ind] != '\0' && ind < size - 1)
	{
		dst[ind] = src[ind];
		ind++;
	}
	dst[ind] = '\0';
	return (ft_strlen(src));
}
