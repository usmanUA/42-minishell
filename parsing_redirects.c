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
#include "vec/vec.h"

int ft_init_vecs(t_redirect *redirect, t_input **input, t_vars *vars)
{
    t_vec *new_fds;
    t_vec *orig_fds;

    new_fds = (t_vec *)malloc(sizeof(t_vec));
    if (!new_fds)
	return (0);
    orig_fds = (t_vec *)malloc(sizeof(t_vec));
    if (!orig_fds)
	return (0);
    if (!vec_new(new_fds, 2, sizeof(int))) // NOTE: Init a vec and allocate some mem for command
	return (0); // NOTE: malloc fail
    if (!vec_new(orig_fds, 2, sizeof(int))) // NOTE: Init a vec and allocate some mem for command
	return (0); // NOTE: malloc fail
    redirect->new_fds = new_fds;
    redirect->orig_fds = orig_fds;
    redirect->file_flag = &(*input)->file_flag;
    return (1);
}

// TODO: check if decrementing redir_count with correct logic
int ft_input_redir(t_redirect *redirect,  char *filename, char **fd, t_vars *vars)
{
    // NOTE: UPDATES redirect based on the possible input redirection
    int std_in;
    int file_fd;
    int redir_pair;

    redir_pair = vars->redir_count->in_redir;
    std_in = 0;
    file_fd = open(filename, O_RDONLY);
    if (file_fd == -1)
    {
	ft_filerror(errno, filename, 1, 1);	
	**redirect->file_flag = RED;
	free(*fd);
	return (1); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    if (redir_pair  == 1 && ((vars->fd && ft_atoi(*fd) == 0) || vars->fd == 0))
    {
	if (!vec_push(redirect->new_fds, &file_fd)) // NOTE: fd_in is in stack mem of this function, be careful
	{
	    if (vars->fd && fd)
		free(*fd);
	    return (0);
	}
	if (!vec_push(redirect->orig_fds, &std_in)) // NOTE: fd_in is in stack mem of this function, be careful
	{
	    if (vars->fd && fd)
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

int ft_output_redir(t_redirect *redirect,  char *filename, char **fd, t_vars *vars)
{
    // NOTE: UPDATES fds based on the possible output redirection
    int file_fd;
    int std_out;
    int redir_pair;
    int redir_fd;

    if (vars->fd)
	redir_fd = ft_atoi(*fd);
    std_out = 1;
    redir_pair = vars->redir_count->out_redir;
    file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd == -1)
    {
	ft_filerror(errno, filename, 0, 0);	
	**redirect->file_flag = RED;
	free(*fd);
	return (1); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    if (redir_pair  == 1 && ((vars->fd && (redir_fd == 1 || redir_fd == 2)) || vars->fd == 0))
    {
	if (!vec_push(redirect->new_fds, &file_fd))
	{
	    if (vars->fd && fd)
		free(*fd);
	    return (0);
	}
	if (redir_fd == 2)
	    std_out = 2;	
	if (!vec_push(redirect->orig_fds, &std_out))
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

int ft_output_append(t_redirect *redirect,  char *filename, char **fd, t_vars *vars)
{
    // NOTE: UPDATES fds based on the possible output append redirection
    int file_fd;
    int std_out;
    int redir_pair;
    int append_fd;

    if (vars->fd)
	append_fd = ft_atoi(*fd);
    redir_pair = vars->redir_count->append;
    std_out = 1;
    file_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (file_fd == -1)
    {
	ft_filerror(errno, filename, 0, 0);	
	**redirect->file_flag = RED;
	free(*fd);
	return (1); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    if (vars->redir_count->out_redir == 0)
    {
	if (redir_pair  == 1 && ((vars->fd && (append_fd == 1 || append_fd == 2)) || vars->fd == 0))
	{
	    if (append_fd == 2)
		std_out = append_fd;
	    if (!vec_push(redirect->new_fds, &file_fd))
	    {
		if (vars->fd && fd)
		    free(*fd);
		return (0);
	    }
	    if (!vec_push(redirect->orig_fds, &std_out)) // NOTE: fd_in is in stack mem of this function, be careful
	    {
		if (vars->fd && fd)
		    free(*fd);
		return (0);
	    }
	}
	else if (vars->redir_count->append > 1)
	    vars->redir_count->append--;
    }
    if (vars->fd && fd)
	free(*fd);
    return (1);
}

int ft_here_doc(t_redirect *redirect,  char *eof, char **fd, t_vars *vars)
{
    // NOTE:UPDATES the fds and reads STDIN and saves the input to hidden file .here_doc 
    char *line;
    int	std_in;
    int fd_here_doc;
    int redir_pair;

    redir_pair = vars->redir_count->in_redir;
    std_in = 0;
    fd_here_doc = open(".infile.txt", O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (fd_here_doc == -1)
    {
	ft_filerror(errno, ".infile.txt", 0, 0);	
	free(*fd);
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
    if (redir_pair  == 1 && ((vars->fd && ft_atoi(*fd) == 0) || vars->fd == 0))
    {
	if (!vec_push(redirect->new_fds, &fd_here_doc)) // NOTE: fd_in is in stack mem of this function, be careful
	{
	    if (vars->fd && fd)
		free(*fd);
	    return (0);
	}
	if (!vec_push(redirect->orig_fds, &std_in))
	{
	    if (vars->fd && fd)
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

int	ft_handle_redirects(t_input **input, t_vars *vars)
{
    // NOTE: checks the string right after the redirect operator is not an invalid operator and if so returns 1
    // OTHERWISE tries to open the files and save their file descriptors and moves the pointer to the next string after filename
    char *redir;
    char *fd;
    char *file;
    t_redirect redirect;

    file = NULL;
    fd = NULL;
    file = NULL;
    if (!ft_init_vecs(&redirect, input, vars))
	return (0);
    if (vars->fd)
    {
	fd = ft_next_string(vars, FD); // NOTE: str in heap, vars->ind points ->FD)<-vars->end 
	if (!fd)
	    return (0);
    }
    vars->ind = vars->end;
    redir = ft_next_string(vars, REDIRECT); // NOTE: str in heap, vars->ind points ->opertr)<-vars->end 
    if (!redir)
	return (0);
    vars->ind = vars->end;
    ft_index_after_spaces(vars);
    if (!ft_save_cmd_filename(vars, &file))
	return (0); // NOTE: malloc fail
    // WARNING: make sure to differentiate malloc fail or file open fail if needed be
    if (!ft_strncmp(redir, "<<", 2))
    {
	if (!ft_here_doc(&redirect, file, &fd, vars))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    else if (!ft_strncmp(redir, ">>", 2))
    {
	if (!ft_output_append(&redirect, file, &fd, vars))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    else if (!ft_strncmp(redir, "<", 1))
    {
	if (!ft_input_redir(&redirect, file, &fd, vars))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    else if (!ft_strncmp(redir, ">", 1))
    {
	if (!ft_output_redir(&redirect, file, &fd, vars))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    if (file)
	free(file);
    if (redir)
	free(redir);
    (*input)->new_fds = redirect.new_fds;
    (*input)->orig_fds = redirect.orig_fds;
   return (1);
}

