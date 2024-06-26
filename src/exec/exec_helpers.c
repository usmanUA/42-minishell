/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 12:32:59 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/22 12:33:00 by uahmed           ###   ########.fr       */
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
	while (++ind < (int)shell->pids->len)
		waitpid(pids[ind], NULL, 0);
}

int	ft_push_pid(t_shell *shell, int *pid, int read_end)
{
	if (!vec_push(shell->pids, pid))
	{
		ft_wait_childs(shell);
		close(read_end);
		return (ft_free_prompt(shell, NO));
	}
	return (SUCCESS);
}

void	ft_free_shell(t_shell *shell)
{
	ft_putstr_fd("exit\n", 2);
	free_env_list(shell);
	free_env_array(shell);
	ft_free_prompt(shell, NO);
}

int	ft_exec_in_child(int exec_type, int tot_pipes)
{
	if (tot_pipes > 0)
		return (YES);
	if (exec_type == EXTERNAL || exec_type == MY_ECHO)
		return (YES);
	return (NO);
}
