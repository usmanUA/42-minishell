/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 14:02:09 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/23 12:42:12 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_vecs(t_vec *pipes);
void	ft_restore_terminal_settings(void);

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	allocate_all_envps(&shell, envp, argc, argv);
	while (42)
	{
		if (ft_prompt(&shell) == FAILURE)
			continue ;
		if (!shell.vars->input_line)
		{
			ft_free_prompt(&shell, NO);
			printf("exit\n");
			break ;
		}
		ft_signals(CHILD, ON);
		if (ft_valid_input(shell.vars, &shell) == NO)
			continue ;
		if (ft_save_input(&shell) == FAILURE)
			continue ;
//		ft_print_vecs(shell.pipes);
		ft_validate_execute(&shell);
	}
	deallocate_all_envps(&shell);
	return (shell.status);
}
