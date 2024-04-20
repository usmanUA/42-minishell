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

int main(void)
{
    t_vec   pipes;
    t_vars	vars;

    while (42)
    {
	ft_init_vars(&vars);
	vars.input_line = readline("MiniWell😎:V0.1$ ");
	add_history(vars.input_line);
	if (ft_syntax_error(&vars))
	{
	    free((char *)vars.input_line);
	    continue ;
	}
	if (!vec_new(&pipes, 1, sizeof(t_input *))) 
	    continue ;
	if (!ft_save_input(&pipes, &vars))
	{
	    free((char *)vars.input_line);
	    continue ;
	}
	free((char *)vars.input_line); // NOTE: everything saved to vector pipes 
	vars.input_line = NULL;
	// TODO: everything is parsed, do:
	// 1. command validation (write errors if there are errors related to the invalid command)
	// 2. if valid command and file, execute command
	// 3. free input.cmd and input.redirect vectors
	// NOTE: Of course there's a lot missing which I did not add to TODO list
    }
}
