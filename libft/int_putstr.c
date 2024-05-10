/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_putstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:36:09 by mkorpela          #+#    #+#             */
/*   Updated: 2024/03/01 11:59:15 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	int_putstr(const char *s)
{
	if (s == NULL)
	{
		return (write(1, "(null)", 6));
	}
	return (write(1, s, ft_strlen(s)));
}
