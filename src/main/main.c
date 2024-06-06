/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 14:02:09 by uahmed            #+#    #+#             */
/*   Updated: 2024/06/05 14:45:14 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	allocate_all_envps(&shell, envp, argc, argv);
	while (42)
	{
		if (ft_prompt(&shell) == FAILURE)
			continue ;
		if (ft_valid_input(shell.vars, &shell) == NO)
			continue ;
		if (ft_save_input(&shell) == FAILURE)
			continue ;
		ft_signals(CHILD, ON);
		ft_validate_execute(&shell);
//		system("leaks -q minishell | grep -w total >> leaks.txt");
	}
	deallocate_all_envps(&shell);
	return (shell.status);
}
