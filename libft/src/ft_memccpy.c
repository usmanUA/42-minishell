/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:30:26 by uahmed            #+#    #+#             */
/*   Updated: 2023/10/31 19:54:02 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	size_t	ind;

	ind = -1;
	while (++ind < n)
	{
		*(unsigned char *)(dest + ind) = *(unsigned char *)(src + ind);
		if (*(unsigned char *)(src + ind) == (unsigned char)c)
			return (dest + ind + 1);
	}
	return (0);
}
