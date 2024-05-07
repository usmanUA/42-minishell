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

int ft_push_fds(t_input **input, t_vars *vars)
{
    int	redirect;
    int	fd_info;
    int	file_fd;

    redirect = vars->redirection_type;
    fd_info = vars->redirection_type;
    if (vars->fd)
	redirect = ft_atoi(*vars->f_des);
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
int ft_input_redir(t_input **input, t_vars *vars)
{
    // NOTE: UPDATES redirect based on the possible input redirection
    vars->redirection_type = STDIN_FILENO;
    vars->file_fd = open(*vars->file, O_RDONLY);
    if (vars->file_fd == -1)
    {
	ft_filerror(errno, *vars->file, YES);	
	*(*input)->file_flag = RED;
	free(*vars->f_des);
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    if (vars->redir_count->in_redir == 1)
    {
	if (!ft_push_fds(input, vars))
	{
	    if (vars->fd && *vars->f_des)
		free(*vars->f_des);
	    return (0);
	}
    }
    else if (vars->redir_count->in_redir > 1)
	vars->redir_count->in_redir--;
    if (vars->fd && *vars->f_des)
	free(*vars->f_des);
    return (1);
}

int ft_output_redir(t_input **input, t_vars *vars)
{
    // NOTE: UPDATES fds based on the possible output redirection
    vars->redirection_type = STDOUT_FILENO;
    vars->file_fd = open(*vars->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (vars->file_fd == -1)
    {
	ft_filerror(errno, *vars->file, YES); // NOTE: write error when there's no permission for the output file	
	*(*input)->file_flag = RED;
	free(*vars->f_des);
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    if (vars->redir_count->out_redir == 1)
    {
	if (!ft_push_fds(input, vars))
	{
	    if (vars->fd && *vars->f_des)
		free(*vars->f_des);
	    return (0);
	}
    }
    else if (vars->redir_count->out_redir > 1)
	vars->redir_count->out_redir--;
    if (vars->fd && *vars->f_des)
	free(*vars->f_des);
    return (1);
}

int ft_output_append(t_input **input, t_vars *vars)
{
    // NOTE: UPDATES fds based on the possible output append redirection
    vars->redirection_type = STDOUT_FILENO;
    vars->file_fd = open(*vars->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (vars->file_fd == -1)
    {
	ft_filerror(errno, *vars->file, YES);	
	*(*input)->file_flag = RED;
	free(*vars->f_des);
	vars->f_des = NULL;
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    if (vars->redir_count->out_redir == 1)
    {
	if (!ft_push_fds(input, vars))
	{
	    if (vars->fd && *vars->f_des)
		free(*vars->f_des);
	    return (0);
	}
    }
    else if (vars->redir_count->out_redir > 1)
	vars->redir_count->out_redir--;
    if (vars->fd && *vars->f_des)
	free(*vars->f_des);
    return (1);
}

int ft_get_here_doc(t_vars *vars)
{
    char    *line;
    char    *eof;
    int	fd_here_doc;

    eof = *vars->file;
    fd_here_doc = open(".here_doc", O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (fd_here_doc == -1)
    {
	ft_filerror(errno, ".here_doc", NO);	
	free(*vars->f_des);
	vars->f_des = NULL;
	vars->unlink_here_doc = NO;
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
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
    return (1);
}

int ft_open_here_doc(t_input **input, t_vars *vars)
{
    vars->file_fd = open(".here_doc", O_RDONLY);
    if (vars->file_fd == -1)
    {
	ft_filerror(errno, ".here_doc", NO);	
	*(*input)->file_flag = RED;
	free(*vars->f_des);
	vars->f_des = NULL;
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    return (1);
}

int ft_here_doc(t_input **input, t_vars *vars)
{
    // NOTE:UPDATES the fds and reads STDIN and saves the input to hidden file .here_doc 
    vars->redirection_type = STDIN_FILENO;
    if (ft_get_here_doc(vars) == 0)
	return (0);
    if (vars->redir_count->in_redir  == 1)
    {
	if (ft_open_here_doc(input, vars) == 0)
	    return (0);
	vars->unlink_here_doc = YES;
	if (!ft_push_fds(input, vars))
	{
	    if (vars->fd && *vars->f_des)
		free(*vars->f_des);
	    return (0);
	}
    }
    else if (vars->redir_count->in_redir > 1)
    {
	unlink(".here_doc");
	vars->redir_count->in_redir--;
    }
    if (vars->fd && *vars->f_des)
	free(*vars->f_des);
    return (1);
}

int ft_parse_redirect_fds(t_input **input, t_vars *vars)
{
    if (!ft_strncmp(*vars->redir, "<<", 2))
    {
	if (!ft_here_doc(input, vars))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    else if (!ft_strncmp(*vars->redir, ">>", 2))
    {
	if (!ft_output_append(input, vars))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    else if (!ft_strncmp(*vars->redir, "<", 1))
    {
	if (!ft_input_redir(input, vars))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    else if (!ft_strncmp(*vars->redir, ">", 1))
    {
	if (!ft_output_redir(input, vars))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    if (*vars->file)
	free(*vars->file);
    if (*vars->redir)
	free(*vars->redir);
    return (1);
}

char	*ft_parse_filename(t_input **input, t_vars *vars, t_envp *env_vars)
{
    char *file;

    if (!ft_save_cmd_filename(vars, &file, env_vars, FILENAME))
	return (0); // NOTE: malloc fail
    // WARNING: make sure to differentiate malloc fail or file open fail if needed be
    if (vars->expand_it == YES && vars->expanded == NO)
    {
	*(*input)->file_flag = RED;
	if (file)
	    free(file);
	if (*vars->redir)
	    free(*vars->redir);
	return (NULL);
    }
    return (file);
}

int ft_parse_fd(t_vars *vars, char **fd, t_envp *env_vars)
{
    if (vars->fd)
    {
	*fd = ft_next_string(vars, FD, env_vars); // NOTE: str in heap, vars->ind points ->FD)<-vars->end 
	if (!*fd)
	    return (0);
    }
    vars->ind = vars->end;
    return (1);
}

int	ft_handle_redirects(t_input **input, t_vars *vars, t_envp *env_vars)
{
    // NOTE: checks the string right after the redirect operator is not an invalid operator and if so returns 1
    // OTHERWISE tries to open the files and save their file descriptors and moves the pointer to the next string after filename
    char *redir;
    char *fd;
    char *file;

    if (ft_parse_fd(vars, &fd, env_vars) == 0)
	return (0);
    redir = ft_next_string(vars, REDIRECT, env_vars); // NOTE: str in heap, vars->ind points ->opertr)<-vars->end 
    if (!redir)
	return (0);
    vars->ind = vars->end;
    ft_index_after_spaces(vars);
    vars->f_des = &fd;
    vars->redir = &redir;
    file = ft_parse_filename(input, vars, env_vars);
    if (file == NULL)
	return (0);
    vars->file = &file;
    if (ft_parse_redirect_fds(input, vars) == 0)
	return (0);
   return (1);
}

