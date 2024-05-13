/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:07:36 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/06 16:07:38 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_valid_input(t_vars *vars, t_shell *shell)
{
	if (vars->input_line && vars->input_line[0])
		add_history(vars->input_line);
	vars->exit_status = shell->status;
	if (vars->input_line[0] == '\0')
	{
		shell->status = GREEN;
		return (NO);
	}
	if (ft_syntax_error(vars) == YES)
	{
		shell->status = 258;
		free((char *)vars->input_line);
		return (NO);
	}
	if (!vec_new(shell->pipes, 1, sizeof(t_input **)))
		return (NO); // NOTE: malloc fail, error message | code?
	return (YES);
}

int	ft_prompt(t_shell *shell, char **envp)
{
	if (ft_init_shell(shell, envp) == MALLOC_FAIL)
		return (MALLOC_FAIL);
	ft_init_vars(shell->vars);
	ft_signals(PARENT, OFF, &shell->status);
	shell->vars->input_line = readline(PROMPT);
	return (MALLOC_SUCCESS);
}