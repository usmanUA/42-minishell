/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 06:56:08 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/22 08:16:46 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_digits(char *exit_argument)
{
	int	i;
	int	digit_count;

	i = 0;
	if (exit_argument[0] == '+' || exit_argument[0] == '-')
	{
		i++;
	}
	digit_count = 0;
	while (exit_argument[i] >= '0' && exit_argument[i] <= '9')
	{
		i++;
		digit_count++;
	}
	return (digit_count);
}

static int	strcmp_long_max(char *exit_argument)
{
	char	*long_max;
	int		i;
	int		j;

	long_max = "9223372036854775807";
	i = 0;
	j = 0;
	if (exit_argument[i] == '+')
		i++;
	while (exit_argument[i] != '\0' && long_max[j] != '\0')
	{
		if (exit_argument[i] - long_max[j] == 0)
		{
			i++;
			j++;
		}
		else if (exit_argument[i] - long_max[j] < 0)
		{
			i++;
			j++;
		}
		else
			return (1);
	}
	return (0);
}

int	check_if_number_greater_than_long_max(char *exit_argument)
{
	int	digit_count;

	if (exit_argument[0] == '-')
	{
		return (0);
	}
	digit_count = count_digits(exit_argument);
	if (digit_count >= 20)
	{
		error_msg("exit", exit_argument, 1, false);
		return (255);
	}
	if (digit_count == 19)
	{
		if (strcmp_long_max(exit_argument) != 0)
		{
			error_msg("exit", exit_argument, 1, false);
			return (255);
		}
	}
	return (0);
}

static int	strcmp_long_min(char *exit_argument)
{
	char	*long_min;
	int		i;
	int		j;

	long_min = "-9223372036854775808";
	i = 0;
	j = 0;
	while (exit_argument[i] != '\0' && long_min[j] != '\0')
	{
		if (exit_argument[i] - long_min[j] == 0)
		{
			i++;
			j++;
		}
		else if (exit_argument[i] - long_min[j] < 0)
		{
			i++;
			j++;
		}
		else
			return (1);
	}
	return (0);
}

int	check_if_number_smaller_than_long_min(char *exit_argument)
{
	int	digit_count;

	if (exit_argument[0] != '-')
	{
		return (0);
	}
	digit_count = count_digits(exit_argument);
	if (digit_count >= 20)
	{
		error_msg("exit", exit_argument, 1, false);
		return (255);
	}
	if (digit_count == 19)
	{
		if (strcmp_long_min(exit_argument) != 0)
		{
			error_msg("exit", exit_argument, 1, false);
			return (255);
		}
	}
	return (0);
}
