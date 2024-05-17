/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_and_execute.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:45:53 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/30 15:45:54 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static	int	ft_validate_exec_last_child(t_pipex *pipex, t_shell *shell)
{
	shell->vars->malloc_flag = GREEN;
	pipex->cmd_flag = GREEN;
	ft_validate_commands(pipex, shell);
	if (shell->vars->malloc_flag == RED)
		return (MALLOC_FAIL);
	if (shell->status == GREEN)
		shell->status = pipex->cmd_flag;
	if (ft_execute_last_cmd(pipex, shell) == MALLOC_FAIL)
		return (MALLOC_FAIL);
	return (MALLOC_SUCCESS);
}

int	ft_init_pids(t_shell *shell)
{
	t_vec	*pids;

	pids = (t_vec *)malloc(sizeof(t_vec));
	if (!pids)
		return (ft_free_prompt(shell, NO));
	vec_new(pids, 0, sizeof(int));
	shell->pids = pids;
	return (MALLOC_SUCCESS);
}

static	int	ft_validate_exec_childs(t_pipex *pipex, t_shell *shell)
{
	pipex->cmd_flag = GREEN;
	shell->vars->malloc_flag = GREEN;
	ft_validate_commands(pipex, shell);
	if (shell->vars->malloc_flag == RED)
		return (MALLOC_FAIL);
	if (ft_processes(pipex, shell) == MALLOC_FAIL)
		return (MALLOC_FAIL);
	return (MALLOC_SUCCESS);
}

void	ft_validate_execute(t_shell *shell)
{
	t_pipex	pipex;

	ft_init_pipex(&pipex, shell->pipes->len - 1);
	if (pipex.tot_pipes > 0)
	{
		if (ft_init_pids(shell) == MALLOC_FAIL)
			return;
	}
	while (++pipex.idx < shell->pipes->len - 1)
	{
		pipex.input = (t_input **)vec_get(shell->pipes, pipex.idx);
		if (ft_validate_exec_childs(&pipex, shell) == MALLOC_FAIL)
			return;
	}
	pipex.input = (t_input **)vec_get(shell->pipes, pipex.idx);
	shell->status = *(*pipex.input)->file_flag;
	ft_validate_exec_last_child(&pipex, shell);
	ft_free_prompt(shell, NO);
}

