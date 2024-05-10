/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 11:17:55 by mkorpela          #+#    #+#             */
/*   Updated: 2023/11/19 21:47:49 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t				i;
	unsigned const char	*str1;
	unsigned const char	*str2;

	str1 = (unsigned const char *)s1;
	str2 = (unsigned const char *)s2;
	if (n == 0)
	{
		return (0);
	}
	i = 0;
	while (str1[i] == str2[i] && str1[i] != '\0'
		&& str2[i] != '\0' && i + 1 < n)
	{
		i++;
	}
	return (str1[i] - str2[i]);
}
