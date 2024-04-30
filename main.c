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
#include "miniwell.h"
#include "vec/vec.h"
#include <unistd.h>

int ft_valid_user_input(t_vars *vars, t_vec *pipes)
{
    ft_init_vars(vars);
    vars->input_line = readline("\x1b[32mMiniWell\x1b[0m😎:\x1b[31mV0.1\x1b[0m$ ");
    if (!vars->input_line)
	return (0); // NOTE: malloc fail, error message | code?
    add_history(vars->input_line);
    if (ft_syntax_error(vars))
    {
	free((char *)vars->input_line);
	return (0);
    }
    if (!vec_new(pipes, 1, sizeof(t_input **))) 
	return (0); // NOTE: malloc fail, error message | code?
    return (1);
}

void	ft_init_shell(t_shell *shell, char **envp)
{
    t_vec   pipes;
    t_vec   info;
    t_vars  vars;

    shell->pipes = pipes;
    shell->info = info;
    shell->vars = vars;
    shell->envp = envp;
}

int main(int argc, char **argv, char **envp)
{
    t_shell shell;

    ft_init_shell(&shell, envp);
    while (42)
    {
	if (!ft_valid_user_input(&shell.vars, &shell.pipes))
	    continue ;
	if (!ft_save_input(&shell.pipes, &shell.vars))
	{
	    free((char *)shell.vars.input_line);
	    continue ; // NOTE: [malloc fail, what else fails there?], error message | code?
	}
	free((char *)shell.vars.input_line); // NOTE: everything saved to vector pipes 
	// TODO: everything is parsed, do:wq
	// Of course there's a lot missing which I did not add to TODO list
	// NOTE: FREE everything
/* 	ft_print_vecs(&pipes); */
	ft_validate_execute(&shell);
	ft_free_vec(&shell.pipes, &shell.vars); 
    }
}
