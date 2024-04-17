/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:22:28 by uahmed            #+#    #+#             */
/*   Updated: 2023/10/31 19:58:40 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_substr(char const *s, unsigned int st, size_t len)
{
	char	*sbs;
	size_t	ind;
	size_t	s_len;

	if (!s)
		return (0);
	s_len = ft_strlen((char *)s);
	if (st >= s_len)
		return (ft_strdup(""));
	if (len > s_len)
		len = s_len;
	sbs = (char *)malloc(len + 1);
	if (!sbs)
		return (NULL);
	ind = -1;
	while (++ind < len)
		sbs[ind] = s[st + ind];
	sbs[ind] = 0;
	return (sbs);
}
