/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:28:39 by uahmed            #+#    #+#             */
/*   Updated: 2024/03/15 16:28:53 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_wait_pids(t_pipex *ppx, int index)
{
	int	i;

	i = -1;
	while (++i < index)
		waitpid(ppx->pids[i], NULL, 0);
}
