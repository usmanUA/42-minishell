/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:30:43 by uahmed            #+#    #+#             */
/*   Updated: 2023/10/27 12:01:08 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	ft_size(int n)
{
	int	len;

	if (n == 0)
		return (1);
	len = 0;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static char	*ft_givenbr(int n, int size)
{
	char		*num;
	long int	m;

	m = n;
	if (n < 0)
	{
		m *= -1;
		size++;
	}
	num = malloc(size + 1);
	if (!num)
		return (0);
	num[size] = 0;
	while (size--)
	{
		num[size] = m % 10 + 48;
		m /= 10;
	}
	if (n < 0)
		num[0] = '-';
	return (num);
}

char	*ft_itoa(int n)
{
	return (ft_givenbr(n, ft_size(n)));
}
