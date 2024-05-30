/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:52:51 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/16 16:52:52 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_init_strs(char **redir, char **fd, char **file);
int		ft_parse_redirect_fds(t_shell *shell);

void	ft_free_redirect_strs(char **fd, char **redir, char **file)
{
	free(*fd);
	free(*redir);
	free(*file);
}

int	ft_free_redirect_strs_prompt(char **fd, char **redir, char **file,
		t_shell *shell)
{
	ft_free_redirect_strs(fd, redir, file);
	return (ft_free_prompt(shell, YES));
}

char	*ft_parse_filename(t_shell *shell)
{
	char	*file;

	shell->vars->malloc_flag = GREEN;
	if (!ft_strncmp(*shell->vars->redir, "<<", 2))
		shell->vars->heredoc = YES;
	if (ft_save_cmd_filename(shell, &file, FILENAME) == FAILURE)
	{
		shell->vars->malloc_flag = RED;
		return (NULL);
	}
	if (shell->vars->expand_it == YES && shell->vars->expanded == NO)
	{
		*(*shell->input)->file_flag = BROWN;
		shell->vars->file_error = BROWN;
		return (NULL);
	}
	shell->vars->heredoc = NO;
	return (file);
}

int	ft_parse_fd(t_shell *shell, char **fd)
{
	if (shell->vars->fd)
	{
		*fd = ft_next_string(shell, FD);
		if (!*fd)
			return (FAILURE);
	}
	shell->vars->ind = shell->vars->end;
	return (SUCCESS);
}

int	ft_handle_redirects(t_shell *shell)
{
	char	*redir;
	char	*fd;
	char	*file;

	ft_init_strs(&redir, &fd, &file);
	if (ft_parse_fd(shell, &fd) == FAILURE)
		return (FAILURE);
	shell->vars->f_des = &fd;
	redir = ft_next_string(shell, REDIRECT);
	if (redir == NULL)
		return (ft_free_redirect_strs_prompt(&fd, &redir, &file, shell));
	shell->vars->redir = &redir;
	shell->vars->ind = shell->vars->end;
	ft_index_after_spaces(shell->vars);
	file = ft_parse_filename(shell);
	if (shell->vars->malloc_flag == RED && file == NULL)
		return (ft_free_redirect_strs_prompt(&fd, &redir, &file, shell));
	else if (shell->vars->file_error == BROWN && file == NULL)
	{
		ft_free_redirect_strs(&fd, &redir, &file);
		return (NOFILE);
	}
	shell->vars->file = &file;
	return (ft_parse_redirect_fds(shell));
}
