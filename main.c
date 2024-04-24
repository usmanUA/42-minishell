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
#include <stdlib.h>

void	ft_free_vecs(void *vecs, int cmd)	
{

}

void	ft_free_input(void *inpt)
{
    t_input *input;

    input = (t_input *)inpt;
    ft_free_vecs(input, 0);
    ft_free_vecs(input, 1);
    free(input);
}
void	ft_free_vec(t_vec *pipes)
{
   // TODO:
    // LOOP over 
    int ind;
    t_input *input;
    
    ind = -1;
    while (++ind < pipes->len)
	ft_free_input(pipes->mem[ind*pipes->size]);
}

int main(void)
{
    t_vec   pipes;
    t_vars	vars;

    while (42)
    {
	ft_init_vars(&vars);
	vars.input_line = readline("MiniWell😎:V0.1$ ");
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
	vars.input_line = NULL;
	// TODO: everything is parsed, do:
	// 1. command validation (write errors if there are errors related to the invalid command)
	// 2. if valid command and file, execute command
	// 3. free input.cmd and input.redirect vectors
	// NOTE: Of course there's a lot missing which I did not add to TODO list
	// NOTE: FREE everything
	ft_free_vec(&pipes);	
    }
}
