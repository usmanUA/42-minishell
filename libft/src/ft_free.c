/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 12:43:00 by uahmed            #+#    #+#             */
/*   Updated: 2024/03/04 14:27:50 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stdio.h"

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
