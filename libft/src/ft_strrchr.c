/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:52:59 by mkorpela          #+#    #+#             */
/*   Updated: 2023/11/20 14:32:45 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(char *s, int c)
{
	int	i;

	i = 0;
	i = ft_strlen(s);
	while (s[i] != (char)c && i > 0)
	{
		i--;
	}
	if (s[i] == '\0' && s[i] != (char)c)
	{
		return (NULL);
	}
	if (i == 0 && s[i] != (char)c)
	{
		return (NULL);
	}
	else
	{
		return ((char *)&s[i]);
	}
}
