/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_putnbr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 16:31:11 by mkorpela          #+#    #+#             */
/*   Updated: 2024/03/12 13:45:45 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	write_min_int(void)
{
	int	return_value;
	int	print_length;

	return_value = 0;
	print_length = 0;
	return_value = write(1, "-2147483648", 11);
	if (return_value == -1)
		return (-1);
	print_length += return_value;
	return (print_length);
}

static int	change_sign(int *n, int *print_length)
{
	int	return_value;

	return_value = 0;
	return_value = write(1, "-", 1);
	if (return_value == -1)
		return (-1);
	*print_length += return_value;
	*n = (*n) * -1;
	return (*print_length);
}

static int	putnbr_calc(int n, int print_length, int return_value)
{
	if (n == -2147483648)
		return (return_value = write_min_int());
	if (n < 0)
	{
		return_value = change_sign(&n, &print_length);
		if (return_value == -1)
			return (-1);
	}
	if (n > 9)
	{
		return_value = int_putnbr(n / 10);
		if (return_value == -1)
			return (-1);
		print_length += return_value;
		n = n % 10;
	}
	if (n >= 0 && n <= 9)
	{
		n += '0';
		return_value = write (1, &n, 1);
		if (return_value == -1)
			return (-1);
		print_length += return_value;
	}
	return (print_length);
}

int	int_putnbr(int n)
{
	int	return_value;
	int	print_length;

	return_value = 0;
	print_length = 0;
	return (print_length = putnbr_calc(n, print_length, return_value));
}
