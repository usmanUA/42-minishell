/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:52:51 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/16 16:52:52 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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
		return (ft_free_prompt(shell, YES));
	if (!vec_push((*shell->input)->orig_fds, &redirect))
		return (ft_free_prompt(shell, YES));
	// NOTE: fd_in is in stack mem of this function, be careful
	if (redirect == STDERR_FILENO)
		fd_info = STDERR_FILENO;
	if (!vec_push((*shell->input)->fds_info, &fd_info))
		return (ft_free_prompt(shell, YES));
	// NOTE: fd_in is in stack mem of this function, be careful
	return (MALLOC_SUCCESS);
}

// TODO: check if decrementing redir_count with correct logic
int	ft_input_redir(t_shell *shell)
{
	// NOTE: UPDATES redirect based on the possible input redirection
	shell->vars->redirection_type = STDIN_FILENO;
	shell->vars->file_fd = open(*shell->vars->file, O_RDONLY);
	if (shell->vars->file_fd == -1)
	{
		ft_filerror(errno, *shell->vars->file, YES);
		*(*shell->input)->file_flag = BROWN;
		return (FILE_FAIL);
		//WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
	}
	if (shell->vars->redir_count->in_redir == 1)
		return (ft_push_fds(shell));
	else if (shell->vars->redir_count->in_redir > 1)
		shell->vars->redir_count->in_redir--;
	return (MALLOC_SUCCESS);
}

int	ft_output_redir(t_shell *shell)
{
	// NOTE: UPDATES fds based on the possible output redirection
	shell->vars->redirection_type = STDOUT_FILENO;
	shell->vars->file_fd = open(*shell->vars->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (shell->vars->file_fd == -1)
	{
		ft_filerror(errno, *shell->vars->file, YES);
			// NOTE: write error when there's no permission for the output file
		*(*shell->input)->file_flag = BROWN;
		return (FILE_FAIL);
		//WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
	}
	if (shell->vars->redir_count->out_redir == 1)
		return (ft_push_fds(shell));
	else if (shell->vars->redir_count->out_redir > 1)
		shell->vars->redir_count->out_redir--;
	return (MALLOC_SUCCESS);
}

int	ft_output_append(t_shell *shell)
{
	// NOTE: UPDATES fds based on the possible output append redirection
	shell->vars->redirection_type = STDOUT_FILENO;
	shell->vars->file_fd = open(*shell->vars->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (shell->vars->file_fd == -1)
	{
		ft_filerror(errno, *shell->vars->file, YES);
		*(*shell->input)->file_flag = BROWN;
		return (FILE_FAIL);
		//WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
	}
	if (shell->vars->redir_count->out_redir == 1)
		return (ft_push_fds(shell));
	else if (shell->vars->redir_count->out_redir > 1)
		shell->vars->redir_count->out_redir--;
	return (MALLOC_SUCCESS);
}

int	ft_get_here_doc(t_shell *shell)
{
	char	*line;
	char	*eof;
	int		fd_here_doc;

	eof = *shell->vars->file;
	fd_here_doc = open(".here_doc", O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fd_here_doc == -1)
	{
		ft_filerror(errno, ".here_doc", NO);
		shell->vars->unlink_here_doc = NO;
		*(*shell->input)->file_flag = BROWN;
		return (FILE_FAIL);
		//WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
	}
	line = get_next_line(0);
	while (42)
	{
		if (line == NULL)
			return (ft_free_prompt(shell, YES));;
		if (((ft_strlen(line) - 1) == ft_strlen(eof) && !strncmp(line, eof,
					strlen(eof))) || !line)
			break ;
		write(fd_here_doc, line, ft_strlen(line));
		line = get_next_line(0);
	}
	close(fd_here_doc);
	return (MALLOC_SUCCESS);
}

int	ft_open_here_doc(t_input **input, t_vars *vars)
{
	vars->file_fd = open(".here_doc", O_RDONLY);
	if (vars->file_fd == -1)
	{
		ft_filerror(errno, ".here_doc", NO);
		*(*input)->file_flag = BROWN;
		return (FILE_FAIL);
		//WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
	}
	return (1);
}

int	ft_here_doc(t_shell *shell)
{
	int	flag;
	// NOTE:UPDATES the fds and reads STDIN and saves the input to hidden file .here_doc
	shell->vars->redirection_type = STDIN_FILENO;
	flag = ft_get_here_doc(shell);
	if (flag != MALLOC_SUCCESS)
		return (flag);
	if (shell->vars->redir_count->in_redir == 1)
	{
		if (ft_open_here_doc(shell->input, shell->vars) == FILE_FAIL)
			return (FILE_FAIL); 
		shell->vars->unlink_here_doc = YES;
		return (ft_push_fds(shell));
	}
	else if (shell->vars->redir_count->in_redir > 1)
	{
		unlink(".here_doc");
		shell->vars->redir_count->in_redir--;
	}
	return (MALLOC_SUCCESS);
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
	return (MALLOC_SUCCESS);
}

char	*ft_parse_filename(t_shell *shell)
{
	char	*file;

	shell->vars->malloc_flag = GREEN;
	if (ft_save_cmd_filename(shell, &file, FILENAME) == MALLOC_FAIL)
	{
		shell->vars->malloc_flag = RED;
		return (NULL);
	}
	if (shell->vars->expand_it == YES && shell->vars->expanded == NO)
	{
		*(*shell->input)->file_flag = BROWN;
		return (NULL);
	}
	return (file);
}

int	ft_parse_fd(t_shell *shell, char **fd)
{
	if (shell->vars->fd)
	{
		*fd = ft_next_string(shell, FD); // NOTE: str in heap, vars->ind points ->FD)<-vars->end
		if (!*fd)
			return (ft_free_prompt(shell, YES));
	}
	shell->vars->ind = shell->vars->end;
	return (MALLOC_SUCCESS);
}

int	ft_handle_redirects(t_shell *shell)
{
	char	*redir;
	char	*fd;
	char	*file;

	// NOTE: checks the string right after the redirect operator is not an invalid operator and if so returns 1
	// OTHERWISE tries to open the files and save their file descriptors and moves the pointer to the next string after filename
	if (ft_parse_fd(shell, &fd) == MALLOC_FAIL)
		return (MALLOC_FAIL);
	shell->vars->f_des = &fd;
	redir = ft_next_string(shell, REDIRECT);
	if (redir == NULL)
		return (ft_free_prompt(shell, YES));
	shell->vars->redir = &redir;
	shell->vars->ind = shell->vars->end;
	ft_index_after_spaces(shell->vars);
	file = ft_parse_filename(shell);
	if (shell->vars->malloc_flag == RED && file == NULL)
		return (ft_free_prompt(shell, YES)); 
	else if (shell->vars->file_error == BROWN && file == NULL)
		return (FILE_FAIL);
	shell->vars->file = &file;
	return (ft_parse_redirect_fds(shell));
}
