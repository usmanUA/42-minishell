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

// TODO: check if decrementing redir_count with correct logic
// TODO: Check what to push in case of ints
// Either int * or int ?
int ft_input_redir(t_redirect *redirect,  char *filename, char **fd, t_vars *vars)
{
    // NOTE: UPDATES redirect based on the possible input redirection
//    int	*fd_in;
    int temp_fd_in;
    int redir_pair;

    redir_pair = vars->redir_count->in_redir;
    temp_fd_in = open(filename, O_RDONLY);
    if (temp_fd_in == -1)
    {
	ft_filerror(errno, filename, 1, 1);	
	vars->file_error = 1;
	free(*fd);
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    if (redir_pair  == 1 && ((vars->fd && ft_atoi(*fd) == 0) || vars->fd == 0))
    {
	// if (!(fd_in = (int *)malloc(sizeof(int))))
	// {
	//     free(*fd);
	//     return (0);
	// }
	// *fd_in = temp_fd_in;
	if (!vec_push(redirect->new_fds, &temp_fd_in)) // NOTE: fd_in is in stack mem of this function, be careful
	{
	    free(*fd);
	    return (0);
	}
    }
    else if (vars->redir_count->in_redir > 1)
	vars->redir_count->in_redir--;
    free(*fd);
    return (1);
}

int ft_output_redir(t_redirect *redirect,  char *filename, char **fd, t_vars *vars)
{
    // NOTE: UPDATES fds based on the possible output redirection
    // int	*fd_out;
    // int	*orig_fd;
    int temp_fd_out;
    int redir_pair;
    int redir_fd;

    if (vars->fd)
	redir_fd = ft_atoi(*fd);
    redir_pair = vars->redir_count->out_redir;
    temp_fd_out = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd_out == -1)
    {
	ft_filerror(errno, filename, 0, 0);	
	vars->file_error = 1;
	free(*fd);
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    if (redir_pair  == 1 && ((vars->fd && (redir_fd == 1 || redir_fd == 2)) || vars->fd == 0))
    {
	// if (!(fd_out = (int *)malloc(sizeof(int))))
	// {
	//     free(*fd);
	//     return (0);
	// }
	// *fd_out = temp_fd_out;
	if (!vec_push(redirect->new_fds, &temp_fd_out)) // NOTE: fd_in is in stack mem of this function, be careful
	{
	    free(*fd);
	    return (0);
	}
	// if (!(orig_fd = (int *)malloc(sizeof(int))))
	// {
	//     free(*fd);
	//     vec_free(redirect->new_fds);
	//     free(fd_out);
	//     return (0);
	// }
	// *orig_fd = redir_fd;
	if (!vec_push(redirect->orig_fds, &redir_fd)) // NOTE: fd_in is in stack mem of this function, be careful
	{
	    free(*fd);
	    vec_free(redirect->new_fds);
	    // free(fd_out);
	    // free(orig_fd);
	    return (0);
	}
    }
    else if (vars->redir_count->out_redir > 1)
	vars->redir_count->out_redir--;
    free(*fd);
    return (1);
}

int ft_output_append(t_redirect *redirect,  char *filename, char **fd, t_vars *vars)
{
    // NOTE: UPDATES fds based on the possible output append redirection
    // int	*fd_append;
    // int *orig_fd;
    int temp_fd_append;
    int redir_pair;
    int append_fd;

    if (vars->fd)
	append_fd = ft_atoi(*fd);
    redir_pair = vars->redir_count->append;
    temp_fd_append = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (temp_fd_append == -1)
    {
	ft_filerror(errno, filename, 0, 0);	
	vars->file_error = 1;
	free(*fd);
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    if (vars->redir_count->out_redir == 0)
    {
	if (redir_pair  == 1 && ((vars->fd && (append_fd == 1 || append_fd == 2)) || vars->fd == 0))
	{
	 //    if (!(fd_append = (int *)malloc(sizeof(int))))
	 //    {
		// free(*fd);
		// return (0);
	 //    }
	 //    *fd_append = temp_fd_append;
	    if (!vec_push(redirect->new_fds, &temp_fd_append)) // NOTE: fd_in is in stack mem of this function, be careful
	    {
		free(*fd);
		return (0);
	    }
	 //    if (!(orig_fd = (int *)malloc(sizeof(int))))
	 //    {
		// free(*fd);
		// vec_free(redirect->new_fds);
		// free(fd_append);
		// return (0);
	 //    }
	 //    *orig_fd = append_fd;
	    if (!vec_push(redirect->orig_fds, &append_fd)) // NOTE: fd_in is in stack mem of this function, be careful
	    {
		free(*fd);
		vec_free(redirect->new_fds);
		// free(fd_append);
		// free(orig_fd);
		return (0);
	    }
	}
	else if (vars->redir_count->append > 1)
	    vars->redir_count->append--;
    }
    free(*fd);
    return (1);
}

int ft_here_doc(t_redirect *redirect,  char *eof, char **fd, t_vars *vars)
{
    // NOTE:UPDATES the fds and reads STDIN and saves the input to hidden file .here_doc 
    char *line;
//    int	*fd_here_doc;
    int temp_fd_here_doc;
    int redir_pair;

    redir_pair = vars->redir_count->in_redir;
    temp_fd_here_doc = open(".infile.txt", O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (temp_fd_here_doc == -1)
    {
	ft_filerror(errno, ".infile.txt", 0, 0);	
	vars->file_error = 1;
	free(*fd);
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    line = get_next_line(0);
    while (42)
    {
	if (((ft_strlen(line) - 1) == ft_strlen(eof) && !strncmp(line, eof, strlen(eof))) || !line)
		break ;
	write(temp_fd_here_doc, line, ft_strlen(line));
	line = get_next_line(0);
    }
    if (redir_pair  == 1 && ((vars->fd && ft_atoi(*fd) == 0) || vars->fd == 0))
    {
	// if (!(fd_here_doc = (int *)malloc(sizeof(int))))
	// {
	//     free(*fd);
	//     return (0);
	// }
	// *fd_here_doc = temp_fd_here_doc;
	if (!vec_push(redirect->new_fds, &temp_fd_here_doc)) // NOTE: fd_in is in stack mem of this function, be careful
	{
	    free(*fd);
	    return (0);
	}
    }
    else if (vars->redir_count->in_redir > 1)
	vars->redir_count->in_redir--;
    free(*fd);
    return (1);
}

int	ft_handle_redirects(t_vec *new_fds, t_vec *orig_fds, t_vars *vars)
{
    // NOTE: checks the string right after the redirect operator is not an invalid operator and if so returns 1
    // OTHERWISE tries to open the files and save their file descriptors and moves the pointer to the next string after filename
    char *redir;
    char *fd;
    char *file;
    t_redirect redirect;

    redirect.new_fds = new_fds;
    redirect.orig_fds = orig_fds;
    if (vars->fd)
    {
	fd = ft_next_string(vars, FD); // NOTE: str in heap, vars->ind points ->FD)<-vars->end 
	if (!fd)
	    return (0);
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
	return (0); // NOTE: malloc fail
    // WARNING: make sure to differentiate malloc fail or file open fail if needed be
    if (!ft_strncmp(redir, "<<", 2))
    {
	if (!ft_here_doc(&redirect, file, &fd, vars))
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
    else if (!ft_strncmp(redir, ">>", 2))
    {
	if (!ft_output_append(&redirect, file, &fd, vars))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    if (file)
	free(file);
    if (redir)
	free(redir);
   return (1);
}

