/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:19:22 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/26 15:19:26 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_is_a_dir(char *cmd);

static	void	ft_write_progname(void)
{
	ft_putstr_fd(Y, 2);
	ft_putstr_fd("miniwell: ", 2);
	ft_putstr_fd(T, 2);
}

static void	ft_write_error(int errnu, char *filename)
{
	if (errnu == 0)
	{
		ft_write_progname();
		ft_putstr_fd(BR, 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(T, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(G, 2);
		ft_putendl_fd("ambiguous redirect", 2);
		ft_putstr_fd(T, 2);
		return ;
	}
	ft_write_progname();
	ft_putstr_fd(BR, 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(T, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(G, 2);
	ft_putendl_fd(strerror(errnu), 2);
	ft_putstr_fd(T, 2);
}

void	ft_filerror(int errnu, char *filename, int write)
{
	if (write == YES)
	{
		ft_putstr_fd(Y, 2);
		ft_write_error(errnu, filename);
	}
}

void	ft_cmd_error(char *cmd, int permission, int file_exist)
{
	if (!file_exist)
	{
		ft_write_error(errno, cmd);
		return ;
	}
	ft_write_progname();
	ft_putstr_fd(BR, 2);
	if (permission == 1)
	{
		if (cmd[0])
			ft_putstr_fd(cmd, 2);
		ft_putstr_fd(T, 2);
		ft_putstr_fd(G, 2);
		ft_putendl_fd(": command not found", 2);
		ft_putstr_fd(T, 2);
		return ;
	}
	if (permission == 2)
	{
		ft_is_a_dir(cmd);
		return ;
	}
	perror(cmd);
}

int	ft_token_error(char c, int sgle)
{
	if (sgle)
		c = '\'';
	ft_putstr_fd(V, 2);
	printf("syntax error near unexpected token  `%c'\n", c);
	ft_putstr_fd(T, 2);
	return (YES);
}
