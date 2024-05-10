/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 12:26:45 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/09 10:36:01 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniwell.h"

int	echo_command(char **command)// I might change this function -> refactor later
{
	int		i;
	bool	first_word_flag;

	first_word_flag = true;
	if (command[1] == NULL)
	{
		write(1, "\n", 1);
		return (0);
	}
	i = 1;
	if (ft_strcmp(command[1], "-n") == 0)
	{
		i = 2;
	}
	while (command[i])
	{
		if (first_word_flag == false)
		{
			ft_printf(" ");//Use printf
		}
		first_word_flag = false;
		ft_printf("%s", command[i]);//Use write???//handle error
		i++;
	}
	if (ft_strcmp(command[1], "-n") != 0)
	{
		write(1, "\n", 1);	//if FAIL?
	}
	return (0);
}

