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
#include <stdio.h>

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

int main(int argc, char **argv, char **envp)
{
    t_vec   pipes;
    t_vars	vars;
    char **s;
    int ind;
    t_input *input;

    while (42)
    {
	if (!ft_valid_user_input(&vars, &pipes))
	    continue ;
	if (!ft_save_input(&pipes, &vars))
	{
	    free((char *)vars.input_line);
	    continue ; // NOTE: [malloc fail, what else fails there?], error message | code?
	}
	free((char *)vars.input_line); // NOTE: everything saved to vector pipes 
	// TODO: everything is parsed, do:
	// 1. command validation (write errors if there are errors related to the invalid command)
	// 2. if valid command and file, execute command
	// 3. free input.cmd and input.redirect vectors
	// NOTE: Of course there's a lot missing which I did not add to TODO list
	// NOTE: FREE everything
	ft_print_vecs(&pipes);
	input = *(t_input **)vec_get(&pipes, 0);
	s = (char **)vec_get(input->cmd, 0);
	printf("sizeof s: %zu\n", input->cmd->len);
	ind = -1;
	while (s[++ind])
	    printf("arg: %s\n", s[ind]);
	ft_free_vec(&pipes, &vars); 
    }
}
