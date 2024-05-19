/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:38:35 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/16 12:47:38 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_io_redirections(t_pipex *pipex, int redir_type);
int	ft_status(int status);
void	ft_wait_childs(t_shell *shell);
int	ft_push_pid(t_shell *shell, int *pid, int read_end);
void	ft_free_shell(t_shell *shell);
int	ft_exec_in_child(int exec_type, int tot_pipes);

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
	if (pipex->output_to_pipe == YES)
		dup2(pipex->write_end, STDOUT_FILENO);
	if (pipex->err_to_pipe == YES)
		dup2(pipex->write_end, STDERR_FILENO);
	close(pipex->write_end);
	if (*(*pipex->input)->file_flag == GREEN && pipex->cmd_flag == GREEN)
	{
		args = (char **)vec_get((*pipex->input)->cmd, 0);
		if (pipex->exec_type == EXTERNAL)
			execve(args[0], args, shell->envp);
		else
			exit(builtin_commands(shell, args, pipex->exec_type));	
	}
	exit(EXIT_FAILURE);
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
		if (ft_push_pid(shell, &pid, fds[0]) == FAILURE)
			return (ft_free_prompt(shell, NO));
	}
	return (SUCCESS);
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
	if (*(*pipex->input)->file_flag == GREEN && pipex->cmd_flag == GREEN)
	{
		if (pipex->exec_type == EXTERNAL)
			execve(command[0], command, shell->envp);
		else
			exit(builtin_commands(shell, command, pipex->exec_type));
	}
	exit(EXIT_FAILURE);
}

static int	ft_execute_in_child(t_pipex *pipex, t_shell *shell, char **command)
{
	int	pid;
	int	status;

	// NOTE:
	// 1. If there were pipe/s (> 0) STDIN is the previous pipes read end OR 0< file is new STDIN (if there's input redirection)
	// 2. If no pipe/s (pipes == 0),STDIN is STDIN OR 0< file is new STDIN (if there's input redirection)
	pid = fork();
	if (pid == -1)
		return (ft_free_prompt(shell, NO));
	if (pid == 0)
		ft_last_chid(pipex, shell, command);
	else
	{
		waitpid(pid, &status, 0);
		if (shell->status != YELLOW && shell->status != RED)
			shell->status = ft_status(status);
		if (pipex->infile != 42)
			close(pipex->infile);
	}
	return (SUCCESS);
}

int	ft_execute_last_cmd(t_pipex *pipex, t_shell *shell)
{
	char	**args;
	int	malloc_flag;
	
	malloc_flag = SUCCESS;
	// TODO: wait for the prev. childs using their pids (if there are any)
	args = (char **)vec_get((*pipex->input)->cmd, 0);
	if (ft_exec_in_child(pipex->exec_type, pipex->tot_pipes) == YES)
	{
		if (ft_execute_in_child(pipex, shell, args) == FAILURE)
			return (FAILURE);
	}
	else if (*(*pipex->input)->file_flag == GREEN && pipex->cmd_flag == GREEN)
		malloc_flag = builtin_commands(shell, args, pipex->exec_type);
	if (pipex->tot_pipes > 0)
		ft_wait_childs(shell);
	if (malloc_flag == FAILURE)
		return (FAILURE);
	if (pipex->tot_pipes == 0)
	{
		if (pipex->exec_type == EXIT)
		{
			ft_free_shell(shell);
			exit(shell->status);
		}
	}
	return (SUCCESS);
}
