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
#include <stdio.h>
#include <unistd.h>

int	ft_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (1);
}

static void	ft_exec_child(t_input *input, int *fds)
{
	char	**args;
	int	write_to_pipe;
	int	*fds_info;
	int	ind;
	int	out_fd;

	ind = -1;
	write_to_pipe = 1;
	fds_info = (int *)vec_get(input->fds_info, 0);
	while (++ind < input->new_fds->len)	
	{
		if (fds_info[ind] == STDOUT_FILENO)
		{
			out_fd = *(int *)vec_get(input->new_fds, ind);
			if (out_fd == STDOUT_FILENO)
				write_to_pipe = 0;
			dup2(out_fd, *(int *)vec_get(input->orig_fds, ind));
		}
	}
	if (write_to_pipe)
		dup2(fds[1], STDOUT_FILENO);
	args = (char **)vec_get(input->cmd, 0);
	close(fds[1]);
	execve(args[0], args, NULL);
	// TODO: check for execve fail
}

static void	ft_child(t_input *input, t_pipex *pipex, int *fds_info, int *fds)
{
	int	ind;

	ind = -1;
	if (pipex->idx == 0) // NOTE: ONLY dup2 when asked to
	{
		while (++ind < input->new_fds->len)	
		{
			if (fds_info[ind] == STDIN_FILENO)
				dup2(*(int *)vec_get(input->new_fds, ind), *(int *)vec_get(input->orig_fds, ind));
		}
	}
	close(fds[0]);
	ft_exec_child(input, fds);
}

static void	ft_last_child(t_input *input, t_pipex *pipex, char **envp)
{
	// NOTE: 
	// 1. If there were pipe/s (> 0) STDIN is the previous pipes read end OR 0< file is new STDIN (if there's input redirection)
	// 2. If no pipe/s (pipes == 0), STDIN is STDIN OR 0< file is new STDIN (if there's input redirection)
	int	ind;
	int	*fds_info;
	char	**args;
	
	ind = -1;
	if (input->fds_info != NULL) 
	{
		fds_info = (int *)vec_get(input->fds_info, 0);
		while (++ind < input->new_fds->len)	
		{
			if (fds_info[ind] == STDIN_FILENO)
				dup2(*(int *)vec_get(input->new_fds, ind), *(int *)vec_get(input->orig_fds, ind));
			else if (fds_info[ind] == STDOUT_FILENO)
				dup2(*(int *)vec_get(input->new_fds, ind), *(int *)vec_get(input->orig_fds, ind));
		}
	}
	args = (char **)vec_get(input->cmd, 0);
	execve(args[0], args, NULL);
}

static void	ft_processes(t_input *input, t_pipex *pipex)
{
	int	fds[2];
	int	pid;
	int	ind;
	int	*fds_info;

	ind = -1;
	fds_info = (int *)vec_get(input->fds_info, 0);
	if (pipe(fds) == -1)
	{
		// TODO: bring back the prompt
	}
	pid = fork();
	if (pid == -1)
	{
		// TODO: Handle fork fail
	}
	if (!vec_push(pipex->pids, &pid))
	{
		// TODO: malloc fail
	}
	if (pid == 0)
		ft_child(input, pipex, fds_info, fds);
	else
	{
		close(fds[1]);
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
	}
}

int	ft_execute(t_vec *pipes)
{
	t_pipex pipex;
	t_vec	pids;
	int	pid;

	pipex.idx = -1;
	pipex.status = 0;
	if (!vec_new(&pids, 1, sizeof(int)))
		return (0); // NOTE: malloc fail
	pipex.pids = &pids;
	while (++pipex.idx < pipes->len - 1)
		ft_processes(*(t_input **)vec_get(pipes, pipex.idx), &pipex);
	pid = fork();
	if (pid == -1)
	{
		// TODO: Hanlde fork fail
	}
	if (!vec_push(pipex.pids, &pid))
	{
		// TODO: malloc fail
	}
	if (pid == 0)
		ft_last_child(*(t_input **)vec_get(pipes, pipex.idx), &pipex, NULL);
	else
		waitpid(pid, &pipex.status, 0);
	pipex.status = ft_status(pipex.status);
	return (pipex.status); // TODO: somehow return this status when command is valid
}
