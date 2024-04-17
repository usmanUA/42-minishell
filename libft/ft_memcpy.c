/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 12:21:45 by uahmed            #+#    #+#             */
/*   Updated: 2023/10/25 21:01:24 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	ind;

	if (!src && !dest)
		return (0);
	ind = -1;
	while (++ind < n)
		((unsigned char *)dest)[ind] = ((const unsigned char *)src)[ind];
	return (dest);
}
