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
#include "miniwell.h"

int ft_valid_input(t_vars *vars, t_vec *pipes)
{
    if (vars->input_line && vars->input_line[0])
	add_history(vars->input_line);
    if (ft_syntax_error(vars) == YES)
    {
	free((char *)vars->input_line);
	return (NO);
    }
    if (!vec_new(pipes, 1, sizeof(t_input **))) 
	return (NO); // NOTE: malloc fail, error message | code?
    return (YES);
}

int ft_prompt(t_shell *shell, char **envp)
{
    if (ft_init_shell(shell, envp) == MALLOC_FAIL)
	return (MALLOC_FAIL);
    ft_init_vars(shell->vars);
    ft_parent_signals(0, 0);
    shell->vars->input_line = readline(PROMPT);
    return (MALLOC_SUCCESS);
}

