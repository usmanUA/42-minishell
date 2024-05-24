/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_opening.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 10:27:58 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/22 10:28:01 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_shift_pointer(t_shell *shell);
int		ft_push_fds(t_shell *shell);
int		ft_get_here_doc(t_shell *shell, char *eof);
void	ft_free_redirect_strs(char **fd, char **redir, char **file);
int		ft_push_here_doc(t_shell *shell);

static int	ft_here_doc(t_shell *shell)
{
	int	flag;

	ft_signals(CHILD, OFF, &shell->status);
	shell->vars->redirection_type = STDIN_FILENO;
	flag = ft_get_here_doc(shell, *shell->vars->file);
	if (flag != SUCCESS)
	{
		if (flag == NOFILE)
		{
			ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir,
				shell->vars->file);
		}
		return (flag);
	}
	if (shell->vars->redir_count->in_redir == 1)
		return (ft_push_here_doc(shell));
	else if (shell->vars->redir_count->in_redir > 1)
	{
		unlink(".here_doc");
		shell->vars->redir_count->in_redir--;
	}
	ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir,
		shell->vars->file);
	return (SUCCESS);
}

static int	ft_input_redir(t_shell *shell)
{
	shell->vars->redirection_type = STDIN_FILENO;
	shell->vars->file_fd = open(*shell->vars->file, O_RDONLY);
	if (shell->vars->file_fd == -1)
	{
		ft_filerror(errno, *shell->vars->file, YES);
		*(*shell->input)->file_flag = BROWN;
		ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir,
			shell->vars->file);
		ft_shift_pointer(shell);
		return (NOFILE);
	}
	if (shell->vars->redir_count->in_redir == 1)
		return (ft_push_fds(shell));
	else if (shell->vars->redir_count->in_redir > 1)
		shell->vars->redir_count->in_redir--;
	ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir,
		shell->vars->file);
	return (SUCCESS);
}

static int	ft_output_redir(t_shell *shell)
{
	shell->vars->redirection_type = STDOUT_FILENO;
	shell->vars->file_fd = open(*shell->vars->file,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (shell->vars->file_fd == -1)
	{
		ft_filerror(errno, *shell->vars->file, YES);
		*(*shell->input)->file_flag = BROWN;
		ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir,
			shell->vars->file);
		ft_shift_pointer(shell);
		return (NOFILE);
	}
	if (shell->vars->redir_count->out_redir == 1)
		return (ft_push_fds(shell));
	else if (shell->vars->redir_count->out_redir > 1)
		shell->vars->redir_count->out_redir--;
	ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir,
		shell->vars->file);
	return (SUCCESS);
}

static int	ft_output_append(t_shell *shell)
{
	shell->vars->redirection_type = STDOUT_FILENO;
	shell->vars->file_fd = open(*shell->vars->file,
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (shell->vars->file_fd == -1)
	{
		ft_filerror(errno, *shell->vars->file, YES);
		*(*shell->input)->file_flag = BROWN;
		ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir,
			shell->vars->file);
		ft_shift_pointer(shell);
		return (NOFILE);
	}
	if (shell->vars->redir_count->out_redir == 1)
		return (ft_push_fds(shell));
	else if (shell->vars->redir_count->out_redir > 1)
		shell->vars->redir_count->out_redir--;
	ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir,
		shell->vars->file);
	return (SUCCESS);
}

int	ft_parse_redirect_fds(t_shell *shell)
{
	if (!ft_strncmp(*shell->vars->redir, "<<", 2))
		return (ft_here_doc(shell));
	else if (!ft_strncmp(*shell->vars->redir, ">>", 2))
		return (ft_output_append(shell));
	else if (!ft_strncmp(*shell->vars->redir, "<", 1))
		return (ft_input_redir(shell));
	else if (!ft_strncmp(*shell->vars->redir, ">", 1))
		return (ft_output_redir(shell));
	ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir,
		shell->vars->file);
	return (SUCCESS);
}
