/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_specifier_logic.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:17:05 by mkorpela          #+#    #+#             */
/*   Updated: 2024/02/08 12:13:52 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	format_percent_char_str_and_signed_ints(const char *s, va_list args)
{
	int	return_value;

	return_value = 0;
	if (*s == '%')
		return_value = int_putchar('%');
	else if (*s == 'c')
		return_value = int_putchar(va_arg(args, int));
	else if (*s == 's')
		return_value = int_putstr(va_arg(args, const char *));
	else if (*s == 'd' || *s == 'i')
		return_value = int_putnbr(va_arg(args, int));
	return (return_value);
}

static int	format_uint_and_hexs(const char *s, va_list args, int *print_length)
{
	int	return_value;

	return_value = 0;
	if (*s == 'u')
		return_value = unsigned_int_putnbr(va_arg(args, unsigned int));
	else if (*s == 'p')
	{
		return_value = write(1, "0x", 2);
		if (return_value == -1)
			return (-1);
		*print_length += return_value;
		return_value = convert_long_to_hex(va_arg(args, unsigned long));
	}
	else if (*s == 'x')
		return_value = uint_to_lowercase_hex(va_arg(args, unsigned int));
	else if (*s == 'X')
		return_value = uint_to_uppercase_hex(va_arg(args, unsigned int));
	return (return_value);
}

int	format_specifier_logic(const char *s, va_list args, int return_value)
{
	int	print_length;

	print_length = 0;
	while (*s)
	{
		if (*s == '%')
		{
			s++;
			if (*s == '%' || *s == 'c' || *s == 's' || *s == 'd' || *s == 'i')
				return_value = format_percent_char_str_and_signed_ints(s, args);
			else if (*s == 'u' || *s == 'p' || *s == 'x' || *s == 'X')
				return_value = format_uint_and_hexs(s, args, &print_length);
			else if (*s == '\0')
				return (print_length += return_value);
			else
				return_value = int_putchar(*s);
		}
		else
			return_value = int_putchar(*s);
		if (return_value == -1)
			return (-1);
		print_length += return_value;
		s++;
	}
	return (print_length);
}
