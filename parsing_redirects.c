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
#include "miniwell.h"

int ft_input_redir(t_redirect *redirect,  char *filename, char *fd)
{
    // NOTE: UPDATES redirect based on the possible input redirection
    if redirect->orig_fd = STDIN_FILENO;
    if (fd)
	redirect->orig_fd = ft_atoi(fd);
    redirect->new_fd = open(filename, O_RDONLY);
    if (redirect->new_fd == -1)
    {
	ft_filerror(errno, filename, 1, 1);	
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    return (1);
}

int ft_output_redir(t_redirect *redirect,  char *filename, char *fd)
{
    // NOTE: UPDATES fds based on the possible output redirection
    redirect->orig_fd = STDOUT_FILENO;
    if (fd)
	redirect->orig_fd = ft_atoi(fd);
    redirect->new_fd = open(filename, O_RDONLY | O_CREAT | O_TRUNC, 0644);
    if (redirect->new_fd == -1)
    {
	ft_filerror(errno, filename, 0, 0);	
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    return (1);
}

int ft_output_append(t_redirect *redirect,  char *filename, char *fd)
{
    // NOTE: UPDATES fds based on the possible output append redirection
    redirect->orig_fd = STDOUT_FILENO;
    if (fd)
	redirect->orig_fd = ft_atoi(fd);
    redirect->new_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (redirect->new_fd == -1)
    {
	ft_filerror(errno, filename, 0, 0);	
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    return (1);
}

int ft_here_doc(t_redirect *redirect,  char *eof, char *fd)
{
    // NOTE:UPDATES the fds and reads STDIN and saves the input to hidden file .here_doc 
    char *line;

    redirect->orig_fd = STDIN_FILENO;
    if (fd)
	redirect->orig_fd = ft_atoi(fd);
    redirect->new_fd = open(".infile.txt", O_WRONLY | O_APPEND | O_CREAT, 0666);
    if (redirect->new_fd == -1)
    {
	ft_filerror(errno, ".infile.txt", 0, 0);	
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    line = get_next_line(0);
    while (42)
    {
	if (((ft_strlen(line) - 1) == ft_strlen(eof) && !strncmp(line,
				eof, strlen(eof))) || !line)
		break ;
	write(redirect->new_fd, line, ft_strlen(line));
	line = get_next_line(0);
    }
    return (1);
}

int ft_save_last_fds()
{
    return (1);
}

int	ft_handle_redirects(t_redirect *redirect, t_vars *vars)
{
    // NOTE: checks the string right after the redirect operator is not an invalid operator and if so returns 1
    // OTHERWISE tries to open the files and save their file descriptors and moves the pointer to the next string after filename
    char *redir;
    char *fd;
    char *file;

    if (vars->fd)
    {
	fd = ft_next_string(vars, FD); // NOTE: str in heap, vars->ind points ->FD)<-vars->end 
	if (!fd)
	{
	    return (0);
	}
    }
    vars->ind = vars->end;
    redir = ft_next_string(vars, REDIRECT); // NOTE: str in heap, vars->ind points ->opertr)<-vars->end 
    if (!redir)
    {
	return (0);
    }
    vars->ind = vars->end;
    ft_index_after_spaces(vars);
    if (!ft_save_cmd_filename(vars, &file))
    {
	if (redir)	
	    free(redir);
	if (fd)
	    free(fd);
	return (0); // NOTE: malloc fail
    }
    // WARNING: make sure to differentiate malloc fail or file open fail if needed be
    if (!ft_strncmp(redir, "<<", 2))
    {
	if (!ft_here_doc(redirect, file, fd))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    else if (!ft_strncmp(redir, "<", 1))
    {
	if (!ft_input_redir(redirect, file, fd))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    else if (!ft_strncmp(redir, ">", 1))
    {
	if (!ft_output_redir(redirect, file, fd))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    else if (!ft_strncmp(redir, ">>", 2))
    {
	if (!ft_output_append(redirect, file, fd))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    if (file)
	free(file);
    if (redir)
	free(redir);
   return (1);
}

