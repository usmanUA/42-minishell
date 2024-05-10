/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 08:24:55 by mkorpela          #+#    #+#             */
/*   Updated: 2023/11/19 21:48:05 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*cpy_s(unsigned int start, size_t sub_len, char const *s, char *sub)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0' && i < sub_len)
	{
		sub[i] = s[start];
		i++;
		start++;
	}
	sub[i] = '\0';
	return (sub);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	size_t	sub_len;
	size_t	max_substr_len;
	char	*sub;

	s_len = 0;
	sub_len = 0;
	max_substr_len = 0;
	s_len = ft_strlen(s);
	max_substr_len = s_len - start;
	if (start < 0 || start >= s_len)
		max_substr_len = 0;
	if (len <= max_substr_len)
		sub_len = len;
	else
		sub_len = max_substr_len;
	sub = (char *)malloc((sub_len + 1) * sizeof(char));
	if (sub == NULL)
		return (NULL);
	return (cpy_s(start, sub_len, s, sub));
}
