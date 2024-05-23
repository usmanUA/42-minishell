/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 08:05:24 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/23 12:42:15 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	free_failed_2d_array(t_shell *shell, char **envp, int i)
{
	i--;
	while (i >= 0)
	{
		free(envp[i]);
		i--;
	}
	free(envp);
	shell->envp = NULL;
	return (1);
}

void	free_env_array(t_shell *shell)
{
	int	i;

	if (shell->envp == NULL)
	{
		return ;
	}
	i = 0;
	while (shell->envp[i])
	{
		free(shell->envp[i]);
		i++;
	}
	free(shell->envp);
	shell->envp = NULL;
}

void	deallocate_all_envps(t_shell *shell)
{
	free_env_list(shell);
	free_env_array(shell);
}
