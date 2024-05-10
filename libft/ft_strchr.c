/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 08:58:01 by mkorpela          #+#    #+#             */
/*   Updated: 2023/11/20 14:13:46 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != (char)c && s[i] != '\0')
	{
		i++;
	}
	if (s[i] == '\0' && s[i] != (char)c)
	{
		return (NULL);
	}
	else
	{
		return ((char *)&s[i]);
	}
}
