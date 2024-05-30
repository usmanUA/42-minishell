/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 12:26:45 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/28 11:49:51 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo_command(char **command)
{
	int		i;
	bool	first_word_flag;

	first_word_flag = true;
	if (command[1] == NULL)
	{
		printf("\n");
		return (0);
	}
	i = 1;
	if (ft_strcmp(command[1], "-n") == 0)
		i = 2;
	while (command[i])
	{
		if (first_word_flag == false)
		{
			printf(" ");
		}
		first_word_flag = false;
		printf("%s", command[i]);
		i++;
	}
	if (ft_strcmp(command[1], "-n") != 0)
		printf("\n");
	return (0);
}
