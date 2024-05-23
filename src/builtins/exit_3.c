/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 06:56:06 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/22 08:16:49 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_too_many_arguments(char **command)
{
	int	num_of_args;

	num_of_args = 0;
	while (command[num_of_args])
	{
		num_of_args++;
	}
	num_of_args -= 1;
	if (num_of_args > 1)
	{
		error_msg("exit", NULL, 2, false);
		return (1);
	}
	return (0);
}
