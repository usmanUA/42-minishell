/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 12:51:39 by mkorpela          #+#    #+#             */
/*   Updated: 2024/06/05 13:08:55 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_for_only_digits_and_both_signs(char *exit_argument)
{
	int	i;

	i = 0;
	while (exit_argument[i] != '\0')
	{
		if (exit_argument[i] >= '0' && exit_argument[i] <= '9')
		{
			i++;
		}
		else if (exit_argument[i] == '+' || exit_argument[i] == '-')
		{
			i++;
		}
		else
		{
			error_msg("exit", exit_argument, 1, false);
			return (255);
		}
	}
	return (0);
}

static int	check_sign_is_at_start(char *exit_argument)
{
	if (exit_argument[0] != '+' && exit_argument[0] != '-')
	{
		return (255);
	}
	return (0);
}

static int	check_number_of_signs_and_sign_at_start(char *exit_argument)
{
	int	i;
	int	sign_count;

	i = 0;
	sign_count = 0;
	while (exit_argument[i])
	{
		if (exit_argument[i] == '+' || exit_argument[i] == '-')
			sign_count++;
		i++;
	}
	if (sign_count > 1)
	{
		error_msg("exit", exit_argument, 1, false);
		return (255);
	}
	if (sign_count == 1)
	{
		if (check_sign_is_at_start(exit_argument) != 0)
		{
			error_msg("exit", exit_argument, 1, false);
			return (255);
		}
	}
	return (0);
}

static int	error_handle_exit_argument(t_shell *shell, char **command,
		char *argument)
{
	int	exit_number;

	exit_number = check_for_only_digits_and_both_signs(argument);
	if (exit_number != 0)
	{
		return (exit_number);
	}
	exit_number = check_number_of_signs_and_sign_at_start(argument);
	if (exit_number != 0)
	{
		return (exit_number);
	}
	exit_number = check_if_number_greater_than_long_max(argument);
	if (exit_number != 0)
	{
		return (exit_number);
	}
	exit_number = check_if_number_smaller_than_long_min(argument);
	if (exit_number != 0)
		return (exit_number);
	exit_number = check_if_too_many_arguments(shell, command);
	if (exit_number != 0)
		return (exit_number);
	return (0);
}

int	exit_command(t_shell *shell, char **command)
{
	int		exit_number;
	char	*argument;

	exit_number = 0;
	if (command[1] != NULL)
	{
		argument = ft_strtrim(command[1], " \t");
		if (argument == NULL)
			return (malloc_status_failure(shell));
		exit_number = error_handle_exit_argument(shell, command, argument);
		free(argument);
		if (exit_number == 0)
		{
			exit_number = ft_atoi(command[1]);
		}
	}
	return (exit_number);
}
