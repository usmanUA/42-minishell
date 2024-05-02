/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 13:15:30 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/02 13:15:31 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "miniwell.h"

int ft_init_shell(t_shell *shell, char **envp)
{
    t_vec   *pipes;
    t_vec   *info;
    t_vars  *vars;

    pipes = (t_vec *)malloc(sizeof(t_vec));
    if (!pipes)
	return (0);
    info = (t_vec *)malloc(sizeof(t_vec));
    if (!info)
	return (0);
    vars = (t_vars *)malloc(sizeof(t_vars));
    if (!vars)
	return (0);
    shell->pipes = pipes;
    shell->info = info;
    shell->vars = vars;
    shell->envp = envp; // NOTE: shell->envp once gave segfault in ft_split, be careful carrying this pointer along
    return (1);
}

void	ft_init_vars(t_vars *vars)
{
    // NOTE: Initialize vars struct
    vars->ind = 0;
    vars->end = 0;
    vars->len = 0;
    vars->fd = 0;
    vars->qontinue = 0;
    vars->expand_it = 0;
    vars->s_quote = 0;
    vars->d_quote = 0;
    vars->increment = 0;
    vars->stop = 0;
    vars->file_error = 0;
    vars->unlink_here_doc = NO;
    vars->input_line = NULL;
    vars->redir_count = NULL;
    vars->redirect = NULL;
}

static  int ft_no_redirections(t_input **input, t_redir_count *redir_count)
{
    int redirections;

    redirections = redir_count->in_redir+redir_count->err_redir+redir_count->out_redir;
    if (redirections == 0)
    {
	(*input)->new_fds = NULL;
	(*input)->orig_fds = NULL;
	(*input)->fds_info = NULL;
	return (YES);
    }
    return (NO);
}

int ft_init_redirect_vecs(t_input **input, t_redir_count *redir_count)
{
    t_vec *new_fds;
    t_vec *orig_fds;
    t_vec *fds_info;

    if (ft_no_redirections(input, redir_count) == YES)
        return (1);
    new_fds = (t_vec *)malloc(sizeof(t_vec));
    if (!new_fds)
	return (0);
    (*input)->new_fds = new_fds;
    orig_fds = (t_vec *)malloc(sizeof(t_vec));
    if (!orig_fds)
	return (0);
    (*input)->orig_fds = orig_fds;
    fds_info = (t_vec *)malloc(sizeof(t_vec));
    if (!fds_info)
	return (0);
    (*input)->fds_info = fds_info;
    if (!vec_new(new_fds, 2, sizeof(long))) // NOTE: Init a vec and allocate some mem for command
	return (0); // NOTE: malloc fail
    if (!vec_new(orig_fds, 2, sizeof(long))) // NOTE: Init a vec and allocate some mem for command
	return (0); // NOTE: malloc fail
    if (!vec_new(fds_info, 2, sizeof(long))) // NOTE: Init a vec and allocate some mem for command
	return (0); // NOTE: malloc fail
    return (1);
}

void    ft_init_pipex(t_pipex *pipex)
{
        pipex->idx = -1;
        pipex->status = 0;
        pipex->infile = -42;
        pipex->cmd_flag = GREEN;
        pipex->input = NULL;
}
