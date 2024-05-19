/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:54:12 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/13 13:18:35 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_msg_hardcode(char *command, char *argument, int error_number, bool quotes)
{
	ft_putstr_fd("miniwell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": ", 2);
	if (argument != NULL)
	{
		if (quotes == true)
		{
			ft_putstr_fd("`", 2);
		}
		ft_putstr_fd(argument, 2);
		if (quotes == true)
		{
			ft_putstr_fd("'", 2);
		}		
		ft_putstr_fd(": ", 2);
	}
	if (error_number == 0)
	{
		perror("");
		return ;
	}
	if (error_number == 1)
	{
		ft_putstr_fd("numeric argument required", 2);
	}
	if (error_number == 2)
	{
		ft_putstr_fd("too many arguments", 2);
	}
	if (error_number == 3)
	{
		ft_putstr_fd("Write env - just env. ", 2);
		ft_putstr_fd("No options, arguments or other shenanigans.", 2);
	}
	if (error_number == 4)
	{
		ft_putstr_fd("HOME not set", 2);
	}
	if (error_number == 5)
	{
		ft_putstr_fd("not a valid identifier", 2);
	}
	ft_putstr_fd("\n", 2);
}

void	error_msg_2(int error_number)
{
	if (error_number == 1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd("error retrieving current directory: ", 2);
		ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
		perror("");
	}
}