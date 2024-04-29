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
#include "vec/vec.h"
#include <unistd.h>

int	ft_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (1);
}

int	ft_redirect(t_input *input, int *ind, int fd)
{
	int	*orig_fds;

	orig_fds = (int *)vec_get(input->orig_fds, 0);
	while (++(*ind) < input->orig_fds->len)
	{
		if (orig_fds[*ind] == fd)
			return (1);
	}
	return (0);
}

static void	ft_exec_child(t_input *input, int *fds)
{
	char	**args;
	int	new_fd;
	int	*orig_fds;
	int	ind;

	ind = -1;
	new_fd = fds[1];
	if (ft_redirect(input, &ind, STDOUT_FILENO))
		new_fd = *(int *)vec_get(input->new_fds, ind); // NOTE: ONLY write to the file when asked to 
	args = (char **)vec_get(input->cmd, 0);
	dup2(new_fd, STDOUT_FILENO);
	close(fds[1]);
	execve(args[0], args, NULL);
	// TODO: check for execve fail
}

static void	ft_child(t_input *input, t_pipex *pipex, int *fds)
{
	int ind;

	ind = -1;
	if (pipex->idx == 0 && ft_redirect(input, &ind, STDIN_FILENO)) // NOTE: ONLY dup2 when asked to
	{
		close(fds[0]);
		dup2(*(int *)vec_get(input->new_fds, ind), *(int *)vec_get(input->orig_fds, ind));
		ft_exec_child(input, fds);
	}
	else
	{
		close(fds[0]);
		ft_exec_child(input, fds);
	}
}

static void	ft_last_child(t_input *input, t_pipex *pipex, char **envp)
{
	dup2(ppx->fd_out, STDOUT_FILENO);
	close(ppx->fd_out);
	execve(ppx->cmds[ppx->tot_cmds - 1], ppx->cmd_args[ppx->tot_cmds - 1],
		envp);
}

static void	ft_processes(t_input *input, t_pipex *pipex)
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
	if (!vec_push(pipex->pids, &pid))
	{
		// TODO: malloc fail
	}
	if (pid == 0)
		ft_child(input, pipex, fds);
	else
	{
		close(fds[1]);
		if (!ft_redirect(input, &ind, STDOUT_FILENO))
			dup2(fds[0], STDIN_FILENO); // NOTE: only read from the pipe when the output of the command was not redirected to a file
		close(fds[0]);
	}
}

int	ft_execute_bonus(t_vec *pipes, t_info *info)
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
