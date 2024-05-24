/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_opening_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 10:34:32 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/22 10:34:34 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_redirect_strs(char **fd, char **redir, char **file);
int		ft_free_redirect_strs_prompt(char **fd, char **redir, char **file,
			t_shell *shell);
int	ft_get_line(t_shell *shell, char *eof, int fd);

void	ft_shift_pointer(t_shell *shell)
{
	char	*input_line;
	int		ind;

	ind = shell->vars->end;
	input_line = shell->vars->input_line;
	while (input_line[ind] != '\0' && input_line[ind] != '|')
		ind++;
	shell->vars->ind = ind;
	shell->vars->end = ind;
}

int	ft_push_fds(t_shell *shell)
{
	int	redirect;
	int	fd_info;
	int	file_fd;

	redirect = shell->vars->redirection_type;
	fd_info = shell->vars->redirection_type;
	if (shell->vars->fd)
		redirect = ft_atoi(*shell->vars->f_des);
	file_fd = shell->vars->file_fd;
	if (!vec_push((*shell->input)->new_fds, &file_fd))
		return (ft_free_redirect_strs_prompt(shell->vars->f_des,
				shell->vars->redir, shell->vars->file, shell));
	if (!vec_push((*shell->input)->orig_fds, &redirect))
		return (ft_free_redirect_strs_prompt(shell->vars->f_des,
				shell->vars->redir, shell->vars->file, shell));
	if (redirect == STDERR_FILENO)
		fd_info = STDERR_FILENO;
	if (!vec_push((*shell->input)->fds_info, &fd_info))
		return (ft_free_redirect_strs_prompt(shell->vars->f_des,
				shell->vars->redir, shell->vars->file, shell));
	ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir,
		shell->vars->file);
	return (SUCCESS);
}

int	ft_get_here_doc(t_shell *shell, char *eof)
{
	int		fd_here_doc;

	fd_here_doc = open(".here_doc", O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fd_here_doc == -1)
	{
		ft_filerror(errno, ".here_doc", NO);
		shell->vars->unlink_here_doc = NO;
		*(*shell->input)->file_flag = BROWN;
		return (NOFILE);
	}
	return (ft_get_line(shell, eof, fd_here_doc));
}

int	ft_open_here_doc(t_input **input, t_vars *vars)
{
	vars->file_fd = open(".here_doc", O_RDONLY);
	if (vars->file_fd == -1)
	{
		ft_filerror(errno, ".here_doc", NO);
		*(*input)->file_flag = BROWN;
		return (NOFILE);
	}
	return (SUCCESS);
}

int	ft_push_here_doc(t_shell *shell)
{
	if (ft_open_here_doc(shell->input, shell->vars) == NOFILE)
	{
		ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir,
			shell->vars->file);
		return (NOFILE);
	}
	shell->vars->unlink_here_doc = YES;
	return (ft_push_fds(shell));
}
