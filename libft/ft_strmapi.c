/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:25:02 by uahmed            #+#    #+#             */
/*   Updated: 2023/10/31 15:12:11 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*str;
	unsigned int	ind;

	if (!s || !f)
		return (0);
	str = malloc(ft_strlen(s) + 1);
	if (!s || !str)
		return (0);
	ind = -1;
	while (s[++ind])
		str[ind] = (*f)(ind, s[ind]);
	str[ind] = 0;
	return (str);
}
