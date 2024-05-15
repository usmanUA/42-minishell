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

void	ft_stderr_redirection(t_pipex *pipex, int orig_fd, int index, int redir_type)
{
	int	new_fd;

	new_fd = *(int *)vec_get((*pipex->input)->new_fds, index);
	dup2(new_fd, orig_fd);
	if (redir_type == OUTPUT && orig_fd == STDERR_FILENO)
		pipex->err_to_pipe = NO;
	dup2(pipex->write_end, STDERR_FILENO);
}

void	ft_stdout_redirection(t_pipex *pipex, int orig_fd, int index, int redir_type)
{
	int	new_fd;

	new_fd = *(int *)vec_get((*pipex->input)->new_fds, index);
	dup2(new_fd, orig_fd);
	if (redir_type == OUTPUT && orig_fd == STDOUT_FILENO)
		pipex->output_to_pipe = NO;
}

void	ft_stdin_redirection(t_pipex *pipex, int orig_fd, int index)
{
	int	new_fd;

	new_fd = *(int *)vec_get((*pipex->input)->new_fds, index);
	dup2(new_fd, orig_fd);
	if (orig_fd == STDIN_FILENO)
		pipex->read_from_pipe = NO;
}

void	ft_io_redirections(t_pipex *pipex, int redir_type)
{
	int	ind;
	int	orig_fd;
	int	fds_info;

	ind = -1;
	orig_fd = 42;
	fds_info = 42;
	while ((*pipex->input)->new_fds && ++ind < (int)(*pipex->input)->new_fds->len)
	{
		fds_info = *(int *)vec_get((*pipex->input)->fds_info, ind);
		orig_fd = *(int *)vec_get((*pipex->input)->orig_fds, ind);
		if (fds_info == STDIN_FILENO)
			ft_stdin_redirection(pipex, orig_fd, ind);
		else if (fds_info == STDOUT_FILENO)
			ft_stdout_redirection(pipex, orig_fd, ind, redir_type);
		else if (fds_info == STDERR_FILENO)
			ft_stderr_redirection(pipex, orig_fd, ind, redir_type);
	}
}

static void	ft_child(t_pipex *pipex, t_shell *shell)
{
	char	**args;

	ft_io_redirections(pipex, OUTPUT);
	if (pipex->infile != 42)
	{
		if (pipex->read_from_pipe == YES)
			dup2(pipex->infile, STDIN_FILENO);
		close(pipex->infile);
	}
	close(pipex->read_end);
	// TODO: Check the next four lines carefully
	if (pipex->output_to_pipe == YES)
		dup2(pipex->write_end, STDOUT_FILENO);
	if (pipex->err_to_pipe == YES)
		dup2(pipex->write_end, STDOUT_FILENO);
	close(pipex->write_end);
	args = (char **)vec_get((*pipex->input)->cmd, 0);
	if (pipex->exec_type == EXTERNAL)
		execve(args[0], args, shell->envp);
	else
		exit(builtin_commands(shell, args, pipex->exec_type));	
	exit(EXIT_FAILURE);
}

int	ft_push_pid(t_shell *shell, int *pid, int read_end)
{
	if (!vec_push(shell->pids, pid))
	{
		ft_wait_childs(shell);
		close(read_end);
		return (ft_free_prompt(shell, NO));
	}
	return (MALLOC_SUCCESS);
}

int	ft_processes(t_pipex *pipex, t_shell *shell)
{
	int	pid;
	int	fds[2];

	if (pipe(fds) == -1)
		return (ft_free_prompt(shell, NO));
	pipex->read_end = fds[0];
	pipex->write_end = fds[1];
	pid = fork();
	if (pid == -1)
	{
		close(fds[0]);
		close(fds[1]);
		return (ft_free_prompt(shell, NO));
	}
	if (pid == 0)
		ft_child(pipex, shell);
	else
	{
		close(fds[1]);
		pipex->infile = fds[0];
		if (ft_push_pid(shell, &pid, fds[0]) == MALLOC_FAIL)
			return (ft_free_prompt(shell, NO));
	}
	return (MALLOC_SUCCESS);
}

static	void	ft_last_chid(t_pipex *pipex, t_shell *shell, char **command)
{

	ft_io_redirections(pipex, INPUT);
	if (pipex->infile != 42)
	{
		if (pipex->read_from_pipe == YES)
			dup2(pipex->infile, STDIN_FILENO);
		close(pipex->infile);
	}
	if (pipex->exec_type == EXTERNAL)
		execve(command[0], command, shell->envp);
	else
		exit(builtin_commands(shell, command, pipex->exec_type));
	exit(EXIT_FAILURE);
}

static int	ft_execute_in_child(t_pipex *pipex, t_shell *shell, char **command)
{
	int	pid;

	// NOTE:
	// 1. If there were pipe/s (> 0) STDIN is the previous pipes read end OR 0< file is new STDIN (if there's input redirection)
	// 2. If no pipe/s (pipes == 0),STDIN is STDIN OR 0< file is new STDIN (if there's input redirection)
	pid = fork();
	if (pid == -1)
		return (ft_free_prompt(shell, NO));
	if (pid == 0)
		ft_last_chid(pipex, shell, command);
	else
		waitpid(pid, &shell->status, 0);
	shell->status = ft_status(shell->status);
	return (MALLOC_SUCCESS);
}

void	ft_free_shell(t_shell *shell)
{
	free_env_list(shell);
	ft_free_prompt(shell, NO);
}

int	ft_execute_last_cmd(t_pipex *pipex, t_shell *shell)
{
	int	pid;
	char	**args;

	// TODO: wait for the prev. childs using their pids (if there are any)
	args = (char **)vec_get((*pipex->input)->cmd, 0);
	if (pipex->exec_type == EXTERNAL || (pipex->exec_type != EXTERNAL && pipex->tot_pipes > 0))
	{
		if (ft_execute_in_child(pipex, shell, args) == MALLOC_FAIL)
			return (MALLOC_FAIL);
	}
	else
	{
		if (builtin_commands(shell, args, pipex->exec_type) == MALLOC_FAIL)
			return (MALLOC_FAIL);
	}
	if (pipex->tot_pipes > 0)
		ft_wait_childs(shell);
	if (pipex->exec_type != EXTERNAL && pipex->tot_pipes == 0)
	{
		if (pipex->exec_type == EXIT)
		{
			ft_free_shell(shell);
			exit(EXIT_SUCCESS);
		}
	}
	return (MALLOC_SUCCESS);
}
