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

void	ft_print_vecs(t_vec *pipes)
{
    int ind;
    int i;
    int j;
    t_input *input;
    t_vec *cmd;
    t_vec *redirect;

    ind = -1;
    printf("pipes: len: %zu, alloc_mem: %zu, size: %zu\n", pipes->len, pipes->mem_alloc, pipes->size);
    while (++ind < pipes->len)	
    {
	input = (t_input *)&pipes->mem[ind*pipes->size];
	cmd = input->cmd;
	printf("cmd: len: %zu, alloc_mem: %zu, size: %zu\n", cmd->len, cmd->mem_alloc, cmd->size);
	i = -1;
	// WARNING: DOES not print if the string is longer than let's say 8 chars
	while (++i < cmd->len)
	    printf("cmd and args: %s\n", *(char **)vec_get(cmd, i));
	j = -1;
	/* write(1, "here\n", 5); */
	// while (++j < redirect->len)
	// {
	//     fds = (t_redirect *)vec_get(redirect, j); 
	//     printf("orig fd: %d\n", fds->new_fd);
	// }
    }
}

int main(void)
{
    t_vec   pipes;
    t_vars	vars;

    while (42)
    {
	ft_init_vars(&vars);
	vars.input_line = readline("\x1b[32mMiniWell\x1b[0m😎:\x1b[31mV0.1\x1b[0m$ ");
	if (!vars.input_line)
	    continue ; // NOTE: malloc fail, error message | code?
	add_history(vars.input_line);
	if (ft_syntax_error(&vars))
	{
	    free((char *)vars.input_line);
	    continue ;
	}
	if (!vec_new(&pipes, 1, sizeof(t_input *))) 
	    continue ; // NOTE: malloc fail, error message | code?
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
	//write(1, "here\n", 5);
/* 	ft_free_vec(&pipes); */
    }
}
