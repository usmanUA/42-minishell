/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:43:18 by mkorpela          #+#    #+#             */
/*   Updated: 2024/03/11 17:01:37 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ignore_whitespaces(const char *str)
{
	int	i;

	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
	{
		i++;
	}
	return (i);
}

static int	convert_nb(const char *str, size_t x, int sign, int *i)
{
	while ((str[*i] >= '0' && str[*i] <= '9'))
	{
		if (x > LONG_MAX / 10)
		{
			if (sign == -1)
				return (0);
			return (-1);
		}
		x = x * 10;
		if (x > LONG_MAX / 10 && LONG_MAX % 10 < (str[*i] - '0'))
		{
			if (sign == -1)
				return (0);
			return (-1);
		}
		x = x + (str[*i] - '0');
		(*i)++;
	}
	return (x);
}

int	ft_atoi(const char *str)
{
	int			i;
	int			j;
	long		x;
	int			sign;

	i = 0;
	j = 0;
	x = 0;
	sign = 1;
	i = ignore_whitespaces(str);
	while (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
		j++;
	}
	if (j > 1)
		return (0);
	x = convert_nb(str, x, sign, &i);
	return ((int)(x * sign));
}
