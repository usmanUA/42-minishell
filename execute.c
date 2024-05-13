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
#include "minishell.h"
#include <stdio.h>

int	ft_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (1);
}

void	ft_wait_childs(t_shell *shell)
{
	int	*pids;
	int	ind;

	ind = -1;
	pids = (int *)vec_get(shell->pids, 0);
	while (++ind < shell->pids->len)
		waitpid(pids[ind], NULL, 0);
}

void	ft_stderr_redirection(t_pipex *pipex, int orig_fd, int index, int err_to_pipe)
{
	int	new_fd;

	new_fd = *(int *)vec_get(pipex->input->new_fds, index);
	if (err_to_pipe == YES && orig_fd != STDERR_FILENO)
		dup2(pipex->fds[1], STDERR_FILENO);
	dup2(new_fd, orig_fd);
}

void	ft_stdout_redirection(t_pipex *pipex, int orig_fd, int index, int write_to_pipe)
{
	int	new_fd;

	new_fd = *(int *)vec_get(pipex->input->new_fds, index);
	if (write_to_pipe == YES && orig_fd != STDOUT_FILENO)
		dup2(pipex->fds[1], STDOUT_FILENO);
	dup2(new_fd, orig_fd);
}

void	ft_stdin_redirection(t_pipex *pipex, int orig_fd, int index)
{
	int	new_fd;

	new_fd = *(int *)vec_get(pipex->input->new_fds, index);
	if (pipex->infile != 42)
	{
		if (orig_fd != STDIN_FILENO)
			dup2(pipex->infile, STDIN_FILENO);
		close(pipex->infile);
	}
	dup2(new_fd, orig_fd);
}

void	ft_io_redirections(t_pipex *pipex, int write_to_pipe, int err_to_pipe)
{
	int	ind;
	int	orig_fd;
	int	fds_info;

	ind = -1;
	orig_fd = 42;
	fds_info = 42;
	while (pipex->input->new_fds && ++ind < (int)pipex->input->new_fds->len)
	{
		fds_info = *(int *)vec_get(pipex->input->fds_info, ind);
		orig_fd = *(int *)vec_get(pipex->input->orig_fds, ind);
		if (fds_info == STDIN_FILENO)
			ft_stdin_redirection(pipex, orig_fd, ind);
		else if (fds_info == STDOUT_FILENO)
			ft_stdout_redirection(pipex, orig_fd, ind, write_to_pipe);
		else if (fds_info == STDERR_FILENO)
			ft_stderr_redirection(pipex, orig_fd, ind, err_to_pipe);
	}
}

static void	ft_child(t_pipex *pipex, t_shell *shell)
{
	char	**args;

	close(pipex->fds[0]);
	ft_io_redirections(pipex, YES, YES);
	close(pipex->fds[1]);
	args = (char **)vec_get(pipex->input->cmd, 0);
	if (pipex->exec_type == EXTERNAL)
		execve(args[0], args, shell->envp);
	else
		exit(builtin_commands(shell, args, pipex->exec_type));	
	// TODO: check for execve fail
}

void	ft_push_pid(t_shell *shell, int *pid)
{
	if (!vec_push(shell->pids, pid))
	{
		ft_free_all(shell, YES);
		ft_wait_childs(shell);
		exit(EXIT_FAILURE);
	}
}

void	ft_processes(t_pipex *pipex, t_shell *shell)
{
	int	pid;

	if (pipe(pipex->fds) == -1)
	{
		ft_free_all(shell, YES);
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		// NOTE: should we close the pipes here? fds[0] and fds[1]
		ft_free_all(shell, YES);
		exit(EXIT_FAILURE);
	}
	// TODO: save pids for all the childs (whatever there number is)
	if (pid == 0)
		ft_child(pipex, shell);
	else
	{
		close(pipex->fds[1]);
		pipex->infile = pipex->fds[0];
		ft_push_pid(shell, &pid);
	}
}

static	void	ft_last_chid(t_pipex *pipex, t_shell *shell, char **command)
{

	ft_io_redirections(pipex, NO, NO);
	if (pipex->exec_type == EXTERNAL)
		execve(command[0], command, shell->envp);
	else
		exit(builtin_commands(shell, command, pipex->exec_type));
}

static void	ft_execute_in_child(t_pipex *pipex, t_shell *shell, char **command)
{
	int	pid;

	// NOTE:
	// 1. If there were pipe/s (> 0) STDIN is the previous pipes read end OR 0< file is new STDIN (if there's input redirection)
	// 2. If no pipe/s (pipes == 0),STDIN is STDIN OR 0< file is new STDIN (if there's input redirection)
	pid = fork();
	if (pid == -1)
	{
		ft_free_all(shell, YES);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		ft_last_chid(pipex, shell, command);
	else
		waitpid(pid, &shell->status, 0);
	shell->status = ft_status(shell->status);
}

void	ft_execute_last_cmd(t_pipex *pipex, t_shell *shell)
{
	int	pid;
	char	**args;

	// TODO: wait for the prev. childs using their pids (if there are any)
	args = (char **)vec_get(pipex->input->cmd, 0);
	if (pipex->exec_type == EXTERNAL || (pipex->exec_type != EXTERNAL && pipex->tot_pipes > 0))
		ft_execute_in_child(pipex, shell, args);
	else
		shell->status = builtin_commands(shell, args, pipex->exec_type);
	if (pipex->tot_pipes > 0)
		ft_wait_childs(shell);
	if (pipex->exec_type != EXTERNAL && pipex->tot_pipes == 0)
	{
		if (pipex->exec_type == EXIT)
		{
			ft_free_all(shell, YES);
			exit(EXIT_SUCCESS);
		}
	}
}
