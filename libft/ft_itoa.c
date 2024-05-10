/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 14:26:33 by mkorpela          #+#    #+#             */
/*   Updated: 2023/11/19 21:44:55 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	check_min_int(int *i, int *j, int *n, char *s)
{
	if (*n == -2147483648)
	{
		s[*i + *j] = 8 + '0';
		(*i)--;
		*n = 214748364;
	}
}

static int	ft_len_num(int y)
{
	int	i;

	i = 0;
	if (y == 0)
	{
		return (1);
	}
	while (y != 0)
	{
		y /= 10;
		i++;
	}
	return (i);
}

static char	*ft_calc_int_to_char(int i, int j, int n, char *s)
{
	int	x;

	x = 0;
	s[i + j] = '\0';
	i--;
	check_min_int(&i, &j, &n, s);
	while (n >= 10)
	{
		x = n % 10;
		s[i + j] = x + '0';
		i--;
		n /= 10;
		x = 0;
	}
	if (n < 10)
	{
		s[i + j] = n + '0';
		i--;
	}
	if (j == 1)
		s[i + j] = '-';
	return (s);
}

char	*ft_itoa(int n)
{
	char	*s;
	int		i;
	int		j;
	int		y;

	i = 0;
	j = 0;
	y = n;
	if (n < 0)
	{
		n *= -1;
		j++;
	}
	i = ft_len_num(y);
	s = (char *)malloc((i + j + 1) * sizeof(char));
	if (s == NULL)
	{
		return (NULL);
	}
	s = ft_calc_int_to_char(i, j, n, s);
	return (s);
}
