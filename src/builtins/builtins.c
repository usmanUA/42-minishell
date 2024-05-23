/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:26:15 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/22 10:51:27 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_commands(t_shell *shell, char **command, int exec_type)
{
	int	status;

	status = GREEN;
	if (exec_type == CD)
		status = cd_command(shell, command);
	else if (exec_type == MY_ECHO)
		status = echo_command(command);
	else if (exec_type == ENV)
		status = env_command(shell, command);
	else if (exec_type == EXIT)
		status = exit_command(command);
	else if (exec_type == EXPORT)
		status = export_command(shell, command);
	else if (exec_type == PWD)
		status = pwd_command();
	else if (exec_type == UNSET)
	{
		status = unset_command(shell, command);
	}
	shell->status = status;
	return (status);
}
