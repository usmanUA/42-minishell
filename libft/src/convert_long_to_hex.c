/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_long_to_hex.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 15:38:06 by mkorpela          #+#    #+#             */
/*   Updated: 2024/02/08 12:13:49 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	convert_long_to_hex(unsigned long num)
{
	int		print_length;
	int		return_value;
	char	*hex_lower_chars;

	hex_lower_chars = "0123456789abcdef";
	print_length = 0;
	return_value = 0;
	if (num >= 16)
	{
		print_length += convert_long_to_hex(num / 16);
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
