/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:24:50 by uahmed            #+#    #+#             */
/*   Updated: 2023/10/31 20:02:27 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	ind;

	if (n == 0)
		return (0);
	ind = 0;
	while ((s1[ind] && s1[ind] == s2[ind]) && ind < n)
		ind++;
	if (ind == n)
		ind--;
	return (((unsigned char *)s1)[ind] - ((unsigned char *)s2)[ind]);
}
