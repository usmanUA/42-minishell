/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ispresent.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 10:54:34 by uahmed            #+#    #+#             */
/*   Updated: 2024/03/05 15:39:09 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_ispresent(char *str, char c)
{
	size_t	ind;
	size_t	len;

	ind = -1;
	len = ft_strlen(str);
	while (++ind < len)
	{
		if (str[ind] == c)
			return (1);
	}
	return (0);
}
