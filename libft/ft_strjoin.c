/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:26:06 by uahmed            #+#    #+#             */
/*   Updated: 2023/10/31 20:07:57 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*nstr;
	size_t	i;
	size_t	j;
	size_t	s1_l;
	size_t	s2_l;

	if (!s1 || !s2)
		return (0);
	s1_l = ft_strlen(s1);
	s2_l = ft_strlen(s2);
	if (!s1 && !s2)
		return (0);
	nstr = (char *)malloc(s1_l + s2_l + 1);
	if (!nstr)
		return (0);
	i = -1;
	while (s1[++i])
		nstr[i] = s1[i];
	j = -1;
	while (s2[++j])
		nstr[i++] = s2[j];
	nstr[i] = '\0';
	return (nstr);
}
