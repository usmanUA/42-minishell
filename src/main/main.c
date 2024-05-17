/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 14:02:09 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/15 14:02:11 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.status = GREEN;
	make_linked_list_of_envp(&shell, envp);
	while (42)
	{
		if (ft_prompt(&shell, envp) == MALLOC_FAIL)
			continue ;
		if (!shell.vars->input_line)
		{
			ft_free_prompt(&shell, NO);
			break ; // NOTE: readlines malloc fail? error message | code?
		}
		ft_signals(CHILD, ON, &shell.status);
		if (ft_valid_input(shell.vars, &shell) == NO)
			continue ;
		if (ft_save_input(&shell) == MALLOC_FAIL)
			continue;
//		ft_print_vecs(shell.pipes);
		ft_validate_execute(&shell);
	}
	free_env_list(&shell);
	free_env_array(&shell);
	return (shell.status);
}
