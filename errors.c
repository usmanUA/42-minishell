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
#include "miniwell.h"

static void	ft_write_error(int errnu, char *filename)
{
	if (errnu == 0)
	{
		ft_putstr_fd("miniwell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd("ambiguous redirect", 2);
		return ;
	}
	ft_putstr_fd("miniwell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errnu), 2);
}

void	ft_filerror(int errnu, char *filename, int write)
{
	if (write == YES)
	{
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
	ft_putstr_fd("miniwell: ", 2);
	if (permission == 1)
	{
		if (cmd[0])
			ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": command not found", 2);
		return ;
	}
	if (permission == 2)
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": is a directory", 2);
		return ;
	}
	perror(cmd);
}

int ft_token_error(char c, int sgle)
{
    if (sgle)
	c = '\'';
    printf("bash: syntax error near unexpected token  '%c'\n",c); 
    return (1);
}

// void	ft_exit_error(t_pipex *pipex, int free, int status, int wait_index)
// {
// 	if (wait_index)
// 		ft_wait_pids(pipex, wait_index);
// 	if (free)
// 	{
// 		perror("pipex: ");
// 		ft_free_pipex(pipex);
// 	}
// 	exit(status);
// }
