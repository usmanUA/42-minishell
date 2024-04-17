/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:26:19 by uahmed            #+#    #+#             */
/*   Updated: 2023/10/26 17:48:33 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*nstr;
	int		ind;

	nstr = malloc(ft_strlen((char *)s) + 1);
	if (!nstr)
		return (NULL);
	ind = -1;
	while (s[++ind])
		nstr[ind] = s[ind];
	nstr[ind] = 0;
	return (nstr);
}
