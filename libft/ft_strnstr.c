/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 10:18:48 by mkorpela          #+#    #+#             */
/*   Updated: 2023/11/19 21:47:52 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	needle_len;
	char	*str_haystack;

	str_haystack = (char *)haystack;
	i = 0;
	j = 0;
	needle_len = ft_strlen(needle);
	if (!needle[i])
		return (str_haystack);
	if (!haystack[i] || len < needle_len)
		return (NULL);
	while (i < len && haystack[i] != '\0')
	{
		while (haystack[i + j] == needle[j] && i <= len - needle_len)
		{
			j++;
			if (j == needle_len)
				return (&str_haystack[i]);
		}
		j = 0;
		i++;
	}
	return (NULL);
}
