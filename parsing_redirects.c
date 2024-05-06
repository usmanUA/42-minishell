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

int ft_push_fds(t_input **input, char **fd, t_vars *vars)
{
    int	redirect;
    int	fd_info;
    int	file_fd;

    redirect = vars->redirection_type;
    fd_info = vars->redirection_type;
    if (vars->fd)
	redirect = ft_atoi(*fd);
    file_fd = vars->file_fd;
    if (!vec_push((*input)->new_fds, &file_fd)) // NOTE: fd_in is in stack mem of this function, be careful
	return (0);
    if (!vec_push((*input)->orig_fds, &redirect)) // NOTE: fd_in is in stack mem of this function, be careful
	return (0);
    if (redirect == STDERR_FILENO)
	fd_info = STDERR_FILENO;
    if (!vec_push((*input)->fds_info, &fd_info)) // NOTE: fd_in is in stack mem of this function, be careful
	return (0);
    return (1);
}

// TODO: check if decrementing redir_count with correct logic
int ft_input_redir(t_input **input,  char *filename, char **fd, t_vars *vars)
{
    // NOTE: UPDATES redirect based on the possible input redirection
    vars->redirection_type = STDIN_FILENO;
    vars->file_fd = open(filename, O_RDONLY);
    if (vars->file_fd == -1)
    {
	ft_filerror(errno, filename, YES);	
	*(*input)->file_flag = RED;
	free(*fd);
	return (1); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    if (vars->redir_count->in_redir == 1)
    {
	if (!ft_push_fds(input, fd, vars))
	{
	    if (vars->fd && *fd)
		free(*fd);
	    return (0);
	}
    }
    else if (vars->redir_count->in_redir > 1)
	vars->redir_count->in_redir--;
    if (vars->fd && fd)
	free(*fd);
    return (1);
}

int ft_output_redir(t_input **input,  char *filename, char **fd, t_vars *vars)
{
    // NOTE: UPDATES fds based on the possible output redirection
    vars->redirection_type = STDOUT_FILENO;
    vars->file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (vars->file_fd == -1)
    {
	ft_filerror(errno, filename, YES); // NOTE: write error when there's no permission for the output file	
	*(*input)->file_flag = RED;
	free(*fd);
	return (1); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    if (vars->redir_count->out_redir == 1)
    {
	if (!ft_push_fds(input, fd, vars))
	{
	    if (vars->fd && *fd)
		free(*fd);
	    return (0);
	}
    }
    else if (vars->redir_count->out_redir > 1)
	vars->redir_count->out_redir--;
    if (vars->fd && *fd)
	free(*fd);
    return (1);
}

int ft_output_append(t_input **input,  char *filename, char **fd, t_vars *vars)
{
    // NOTE: UPDATES fds based on the possible output append redirection
    vars->redirection_type = STDOUT_FILENO;
    vars->file_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (vars->file_fd == -1)
    {
	ft_filerror(errno, filename, YES);	
	*(*input)->file_flag = RED;
	free(*fd);
	return (1); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    if (vars->redir_count->out_redir == 1)
    {
    }
    else if (vars->redir_count->out_redir > 1)
	vars->redir_count->out_redir--;
    if (vars->fd && *fd)
	free(*fd);
    return (1);
}

int ft_here_doc(t_input **input,  char *eof, char **fd, t_vars *vars)
{
    // NOTE:UPDATES the fds and reads STDIN and saves the input to hidden file .here_doc 
    char *line;
    int	std_in;
    int	infile;
    int fd_here_doc;
    int	file_fd;

    std_in = 0;
    infile = STDIN_FILENO;
    if (vars->fd)
	std_in = ft_atoi(*fd);
    fd_here_doc = open(".here_doc", O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (fd_here_doc == -1)
    {
	ft_filerror(errno, ".here_doc", NO);	
	free(*fd);
	vars->unlink_here_doc = NO;
	return (1); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    line = get_next_line(0);
    while (42)
    {
	if (((ft_strlen(line) - 1) == ft_strlen(eof) && !strncmp(line, eof, strlen(eof))) || !line)
		break ;
	write(fd_here_doc, line, ft_strlen(line));
	line = get_next_line(0);
    }
    close(fd_here_doc);
    if (vars->redir_count->in_redir  == 1)
    {
	file_fd = open(".here_doc", O_RDONLY);
	if (file_fd == -1)
	{
	    ft_filerror(errno, ".here_doc", NO);	
	    *(*input)->file_flag = RED;
	    free(*fd);
	    return (1); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
	}
	vars->unlink_here_doc = YES;
	if (!vec_push((*input)->new_fds, &file_fd)) // NOTE: fd_in is in stack mem of this function, be careful
	{
	    if (vars->fd && fd)
		free(*fd);
	    return (0);
	}
	if (!vec_push((*input)->orig_fds, &std_in))
	{
	    if (vars->fd && fd)
		free(*fd);
	    return (0);
	}
	if (!vec_push((*input)->fds_info, &infile))
	{
	    if (vars->fd && fd)
		free(*fd);
	    return (0);
	}
    }
    else if (vars->redir_count->in_redir > 1)
    {
	unlink(".here_doc");
	vars->redir_count->in_redir--;
    }
    if (vars->fd && fd)
	free(*fd);
    return (1);
}

int	ft_handle_redirects(t_input **input, t_vars *vars, t_envp *env_vars)
{
    // NOTE: checks the string right after the redirect operator is not an invalid operator and if so returns 1
    // OTHERWISE tries to open the files and save their file descriptors and moves the pointer to the next string after filename
    char *redir;
    char *fd;
    char *file;

    fd = NULL;
    if (vars->fd)
    {
	fd = ft_next_string(vars, FD, env_vars); // NOTE: str in heap, vars->ind points ->FD)<-vars->end 
	if (!fd)
	    return (0);
    }
    vars->ind = vars->end;
    redir = ft_next_string(vars, REDIRECT, env_vars); // NOTE: str in heap, vars->ind points ->opertr)<-vars->end 
    if (!redir)
	return (0);
    vars->ind = vars->end;
    ft_index_after_spaces(vars);
    if (!ft_save_cmd_filename(vars, &file, env_vars, FILENAME))
	return (0); // NOTE: malloc fail
    // WARNING: make sure to differentiate malloc fail or file open fail if needed be
    if (vars->expand_it == YES && vars->expanded == NO)
    {
	*(*input)->file_flag = RED;
	if (file)
	    free(file);
	if (redir)
	    free(redir);
	return (1);
    }
    if (!ft_strncmp(redir, "<<", 2))
    {
	if (!ft_here_doc(input, file, &fd, vars))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    else if (!ft_strncmp(redir, ">>", 2))
    {
	if (!ft_output_append(input, file, &fd, vars))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    else if (!ft_strncmp(redir, "<", 1))
    {
	if (!ft_input_redir(input, file, &fd, vars))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    else if (!ft_strncmp(redir, ">", 1))
    {
	if (!ft_output_redir(input, file, &fd, vars))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    if (file)
	free(file);
    if (redir)
	free(redir);
   return (1);
}

