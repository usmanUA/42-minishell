/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uint_to_uppercase_hex.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 20:56:58 by mkorpela          #+#    #+#             */
/*   Updated: 2024/02/08 12:14:13 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	uint_to_uppercase_hex(unsigned int num)
{
	int		print_length;
	int		return_value;
	char	*hex_upper_chars;

	print_length = 0;
	return_value = 0;
	hex_upper_chars = "0123456789ABCDEF";
	if (num >= 16)
	{
		return_value = uint_to_uppercase_hex(num / 16);
		if (return_value == -1)
			return (-1);
		print_length += return_value;
		num = num % 16;
	}
	if (num <= 15)
	{
		return_value = int_putchar(hex_upper_chars[num]);
		if (return_value == -1)
			return (-1);
		print_length += return_value;
	}
	return (print_length);
}
