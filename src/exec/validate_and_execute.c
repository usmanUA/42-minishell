/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_and_execute.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:45:53 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/22 15:21:35 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_check_builtin(char *command, t_pipex *pipex)
{
	int	executable_info;

	executable_info = EXTERNAL;
	if (ft_strcmp(command, "cd") == 0)
		executable_info = CD;
	if (ft_strcmp(command, "echo") == 0)
		executable_info = MY_ECHO;
	if (ft_strcmp(command, "env") == 0)
		executable_info = ENV;
	if (ft_strcmp(command, "exit") == 0)
		executable_info = EXIT;
	if (ft_strcmp(command, "export") == 0)
		executable_info = EXPORT;
	if (ft_strcmp(command, "pwd") == 0)
		executable_info = PWD;
	if (ft_strcmp(command, "unset") == 0)
		executable_info = UNSET;
	pipex->exec_type = executable_info;
}

static int	ft_validate_exec_last_child(t_pipex *pipex, t_shell *shell)
{
	shell->vars->malloc_flag = GREEN;
	pipex->cmd_flag = GREEN;
	pipex->command = *(char **)vec_get((*pipex->input)->cmd, 0);
	if (pipex->command == NULL)
		return (SUCCESS);
	ft_check_builtin(pipex->command, pipex);
	if (pipex->exec_type == EXTERNAL)
		ft_validate_commands(pipex, shell);
	if (shell->vars->malloc_flag == RED)
		return (FAILURE);
	if (shell->status == GREEN)
		shell->status = pipex->cmd_flag;
	if (ft_execute_last_cmd(pipex, shell) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	ft_init_pids(t_shell *shell)
{
	t_vec	*pids;

	pids = (t_vec *)malloc(sizeof(t_vec));
	if (!pids)
		return (ft_free_prompt(shell, NO));
	vec_new(pids, 0, sizeof(int));
	shell->pids = pids;
	return (SUCCESS);
}

static int	ft_validate_exec_childs(t_pipex *pipex, t_shell *shell)
{
	pipex->cmd_flag = GREEN;
	shell->vars->malloc_flag = GREEN;
	pipex->command = *(char **)vec_get((*pipex->input)->cmd, 0);
	if (pipex->command == NULL)
		return (SUCCESS);
	ft_check_builtin(pipex->command, pipex);
	if (pipex->exec_type == EXTERNAL)
		ft_validate_commands(pipex, shell);
	if (shell->vars->malloc_flag == RED)
		return (FAILURE);
	if (ft_processes(pipex, shell) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

void	ft_validate_execute(t_shell *shell)
{
	t_pipex	pipex;

	ft_init_pipex(&pipex, shell->pipes->len - 1);
	if (pipex.tot_pipes > 0)
	{
		if (ft_init_pids(shell) == FAILURE)
			return ;
	}
	while (++pipex.idx < (int)shell->pipes->len - 1)
	{
		pipex.input = (t_input **)vec_get(shell->pipes, pipex.idx);
		if (ft_validate_exec_childs(&pipex, shell) == FAILURE)
			return ;
	}
	pipex.input = (t_input **)vec_get(shell->pipes, pipex.idx);
	shell->status = *(*pipex.input)->file_flag;
	ft_validate_exec_last_child(&pipex, shell);
	ft_free_prompt(shell, NO);
}
