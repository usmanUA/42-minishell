/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 12:43:00 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/22 07:33:09 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free(char **s, int index)
{
	int	i;

	i = -1;
	if (index)
	{
		while (++i < index)
			free(s[i]);
	}
	else
	{
		while (s[++i])
			free(s[i]);
	}
	free(s);
}
