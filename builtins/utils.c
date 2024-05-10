/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:28:19 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/09 14:30:02 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniwell.h"

char	*string_n_copy(char *dest, char *source, int len)//This is a util in another place
{
	int	i;

	i = 0;
	while (i < len)
	{
		dest[i] = source[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;
	
	// if (s1 == NULL || s2 == NULL)
	// {
	// 	return (-1);
	// }
	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] - s2[i] > 0)
		{
			return (s1[i] - s2[i]);
		}
		if (s1[i] - s2[i] < 0)
		{
			return (s1[i] - s2[i]);
		}
		i++;
	}
	return (0);
}
