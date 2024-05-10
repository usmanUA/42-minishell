/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 12:51:39 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/09 14:17:50 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniwell.h"

int	check_for_only_digits_and_both_signs(char *exit_argument)
{
	int	i;

	i = 0;
	while (exit_argument[i] != '\0')
	if (exit_argument[i] >= '0' && exit_argument[i] <= '9')
	{
		i++;
	}
	else if (exit_argument[i] == '+' || exit_argument[i] == '-')//self-note :Do I need to account for whitespaces also?
	{
		i++;
	}
	else
	{
		error_msg_hardcode("exit", exit_argument, 1, false);
		// ft_write_error(0, "exit");
		// ft_printf("bash: exit: %s: numeric argument required\n", exit_argument);
		// ft_printf("Found a character that is not a digit or sign\n");
		return (255);
	}
	return (0);
}

int	check_sign_is_at_start(char *exit_argument)
{
	if (exit_argument[0] != '+' && exit_argument[0] != '-')
	{
		return (255);
	}
	return (0);
}

int	check_number_of_signs_and_sign_at_start(char *exit_argument)
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
		error_msg_hardcode("exit", exit_argument, 1, false);
		// ft_printf("bash: exit: %s: numeric argument required\n", exit_argument);
		// ft_printf("more than 1 sign\n");
		return (255);
	}
	if (sign_count == 1)
	{
		if (check_sign_is_at_start(exit_argument) != 0)
		{
			error_msg_hardcode("exit", exit_argument, 1, false);
			// ft_printf("bash: exit: %s: numeric argument required\n", exit_argument);
			// ft_printf("Sign not at beginning\n");
			return (255);
		}
	}
	return (0);
}

int	count_digits(char *exit_argument)
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

int	strcmp_long_max(char *exit_argument)
{
	char	*long_max;
	int		i;
	int		j;

	long_max = "9223372036854775807";
	i = 0;
	j = 0;
	if (exit_argument[i] == '+')
	{
		i++;
	}
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
		{
			return (1);
		}
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
	if (digit_count <= 18)//Can delete this "if condition"
	{
		return (0);
	}
	if (digit_count >= 20)
	{
		error_msg_hardcode("exit", exit_argument, 1, false);
		// ft_printf("bash: exit: %s: numeric argument required\n", exit_argument);
		// ft_printf("digit_count is 20 or greater - (long_max func)\n");
		return (255);
	}
	if (digit_count == 19)
	{
		if (strcmp_long_max(exit_argument) != 0)
		{
			error_msg_hardcode("exit", exit_argument, 1, false);
			// ft_printf("bash: exit: %s: numeric argument required\n", exit_argument);
			// ft_printf("Number is greater than LONG_MAX\n");
			return (255);
		}
	}
	return (0);
}

int	strcmp_long_min(char *exit_argument)
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
		{
			return (1);
		}
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
	if (digit_count <= 18)//Can delete this "if condition"
	{
		return (0);
	}
	if (digit_count >= 20)
	{
		error_msg_hardcode("exit", exit_argument, 1, false);
		// ft_printf("bash: exit: %s: numeric argument required\n", exit_argument);
		// ft_printf("digit_count is 20 or greater - (long_min func)\n");
		return (255);
	}
	if (digit_count == 19)
	{
		if (strcmp_long_min(exit_argument) != 0)
		{
			error_msg_hardcode("exit", exit_argument, 1, false);
			// ft_printf("bash: exit: %s: numeric argument required\n", exit_argument);
			// ft_printf("Number is smaller than LONG_MIN\n");
			return (255);
		}
	}
	return (0);
}

int	check_if_too_many_arguments(char **user_input_arr)
{
	int	num_of_args;

	num_of_args = 0;
	while (user_input_arr[num_of_args])
	{
		num_of_args++;
	}
	num_of_args -= 1;
	if (num_of_args > 1)
	{
		error_msg_hardcode("exit", NULL, 2, false);
		// ft_printf("bash: exit: too many arguments\n");//return 1
		//free here
		return (1);//change this so that it returns to prompt - returns.. -or not		
	}
	return (0);
}

int	error_handle_exit_argument(char **command)
{
	int	exit_number;
	
	if (command[1] == NULL)
		return (0);
	exit_number = check_for_only_digits_and_both_signs(command[1]);
	if (exit_number != 0)
	{
		return (exit_number);
	}
	exit_number = check_number_of_signs_and_sign_at_start(command[1]);
	if (exit_number != 0)
	{
		return (exit_number);
	}
	exit_number = check_if_number_greater_than_long_max(command[1]);
	if (exit_number != 0)
		return (exit_number);
	exit_number = check_if_number_smaller_than_long_min(command[1]);
	if (exit_number != 0)
		return (exit_number);
	exit_number = check_if_too_many_arguments(command);
	if (exit_number != 0)
		return (exit_number);
	return (0);
}

void	exit_command(t_shell *data, char **command)
{
	//consider edge case - exit " 2 " -> maybe parser gets rid of double quotes and spaces
	int	exit_number;

	printf("exit\n");
	exit_number = 0;
	exit_number = error_handle_exit_argument(command);
	if (command[1] != NULL)
	{
		if (exit_number == 0)
		{
			exit_number = ft_atoi(command[1]);
		}
	}
	free_all(data);
	exit(exit_number);
}
