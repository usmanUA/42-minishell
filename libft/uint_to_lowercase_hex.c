/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uint_to_lowercase_hex.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 20:34:40 by mkorpela          #+#    #+#             */
/*   Updated: 2024/02/08 12:14:10 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	uint_to_lowercase_hex(unsigned int num)
{
	int		print_length;
	int		return_value;
	char	*hex_lower_chars;

	print_length = 0;
	return_value = 0;
	hex_lower_chars = "0123456789abcdef";
	if (num >= 16)
	{
		return_value = uint_to_lowercase_hex(num / 16);
		if (return_value == -1)
			return (-1);
		print_length += return_value;
		num = num % 16;
	}
	if (num <= 15)
	{
		return_value = int_putchar(hex_lower_chars[num]);
		if (return_value == -1)
			return (-1);
		print_length += return_value;
	}
	return (print_length);
}
