/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:38:35 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/29 14:38:40 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "miniwell.h"
#include <unistd.h>

int	ft_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (1);
}

void	ft_io_redirections(t_input *input, int *read_from_pipe, int *output_to_pipe, int *err_to_pipe)
{
	int	ind;
	int	orig_fd;
	int	fds_info;

	ind = -1;
	orig_fd = 42;
	fds_info = 42;
	while (input->new_fds && ++ind < input->new_fds->len)	
	{
		fds_info = *(int *)vec_get(input->fds_info, ind);
		orig_fd = *(int *)vec_get(input->orig_fds, ind);
		if (fds_info == STDIN_FILENO)
		{
			if (orig_fd == STDIN_FILENO)
				*read_from_pipe = NO;
			dup2(*(int *)vec_get(input->new_fds, ind), orig_fd);
		}
		else if (fds_info == STDOUT_FILENO)
		{
			if (output_to_pipe && orig_fd == STDOUT_FILENO)
				*output_to_pipe = NO;
			dup2(*(int *)vec_get(input->new_fds, ind), orig_fd);
		}
		else if (fds_info == STDERR_FILENO)
		{
			if (err_to_pipe && orig_fd == STDERR_FILENO)
				*err_to_pipe = NO;
			dup2(*(int *)vec_get(input->new_fds, ind), orig_fd);
		}
	}
}

static void	ft_child(t_input *input, t_pipex *pipex, int *fds, char **envp)
{
	int	read_from_pipe;
	int	output_to_pipe;
	int	err_to_pipe;
	char	**args;

	read_from_pipe = YES;
	output_to_pipe = YES;
	err_to_pipe = YES;
	ft_io_redirections(input, &read_from_pipe, &output_to_pipe, &err_to_pipe);
	if (pipex->infile != -42)
	{
		if (read_from_pipe == YES)
			dup2(pipex->infile, STDIN_FILENO);
		close(pipex->infile);
	}
	close(fds[0]);
	if (output_to_pipe == YES)
		dup2(fds[1], STDOUT_FILENO);
	if (err_to_pipe == YES)
		dup2(fds[1], STDERR_FILENO);
	close(fds[1]);
	args = (char **)vec_get(input->cmd, 0);
	execve(args[0], args, envp);
	// TODO: check for execve fail
}

static void	ft_last_child(t_input *input, t_pipex *pipex, char **envp)
{
	// NOTE: 
	// 1. If there were pipe/s (> 0) STDIN is the previous pipes read end OR 0< file is new STDIN (if there's input redirection)
	// 2. If no pipe/s (pipes == 0), STDIN is STDIN OR 0< file is new STDIN (if there's input redirection)
	char	**args;
	int	read_from_pipe;
	
	read_from_pipe = YES;
	ft_io_redirections(input, &read_from_pipe, NULL, NULL);
	if (pipex->infile != -42)
	{
		if (read_from_pipe == YES)
			dup2(pipex->infile, STDIN_FILENO);
		close(pipex->infile);
	}
	args = (char **)vec_get(input->cmd, 0);
	execve(args[0], args, envp);
}

void	ft_processes(t_input *input, t_pipex *pipex, char **envp)
{
	int	fds[2];
	int	pid;
	int	ind;

	ind = -1;
	if (pipe(fds) == -1)
	{
		// TODO: bring back the prompt
	}
	pid = fork();
	if (pid == -1)
	{
		// TODO: Handle fork fail
	}
	// if (!vec_push(pipex->pids, &pid))
	// {
	// 	// TODO: malloc fail
	// }
	if (pid == 0)
		ft_child(input, pipex, fds, envp);
	else
	{
		close(fds[1]);
		pipex->infile = fds[0];
	}
}

int	ft_execute_last_cmd(t_input *input, t_pipex *pipex, char **envp)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		// TODO: Hanlde fork fail
	}
	if (pid == 0)
		ft_last_child(input, pipex, envp);
	else
		waitpid(pid, &pipex->status, 0);
	pipex->status = ft_status(pipex->status);
	return (1);
}

