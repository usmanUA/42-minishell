/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 10:09:00 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/25 10:09:02 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "miniwell.h"

static void	ft_free_redirect(t_vec *redirect)
{
    int ind;

    ind = -1;
    if (redirect)
    {
	while (++ind < redirect->len)
	    close(redirect->mem[ind * redirect->size]);
	vec_free(redirect);
	free(redirect);
    }
}

static void	ft_free_cmd(t_vec *cmd)
{
    char *str;
    int ind;

    ind = -1;
    if (cmd)
    {
	while (++ind < cmd->len)
	{
	    str = *(char **)&cmd->mem[ind*cmd->len];
	    if (str)
		free(str);
	}
	vec_free(cmd);
	free(cmd);
    }
}

static void	ft_free_input(void *inpt)
{
    t_input *input;

    input = (t_input *)inpt;
    if (input)
    {
	ft_free_cmd(input->cmd);
	ft_free_redirect(input->new_fds);
	ft_free_redirect(input->orig_fds);
	free(input);
    }
}
void	ft_free_vec(t_vec *pipes)
{
    int ind;
    t_input *input;
    
    ind = -1;
    while (++ind < pipes->len)
	ft_free_input(&pipes->mem[ind*pipes->size]);
}
