/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 20:35:39 by mkorpela          #+#    #+#             */
/*   Updated: 2024/02/08 09:04:57 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;

	i = 0;
	str = (unsigned char *)s;
	while (i < n && str[i] != (unsigned char)c)
	{
		i++;
	}
	if ((n == i) || str[i] != (unsigned char)c)
	{
		return (NULL);
	}
	else
	{
		return ((void *)&str[i]);
	}
}
