/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:24:29 by uahmed            #+#    #+#             */
/*   Updated: 2023/10/31 19:59:47 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	ind;

	if (!big && !len)
		return (0);
	if (!*little || little == big)
		return ((char *)big);
	ind = ft_strlen(little);
	while (*big && ind <= len--)
	{
		if (!(ft_strncmp(big, little, ind)))
			return ((char *)big);
		big++;
	}
	return (0);
}
