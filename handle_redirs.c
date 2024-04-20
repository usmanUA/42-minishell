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
#include "libft/libft.h"
#include "miniwell.h"
#include "vec/vec.h"


int ft_input_redir(t_vec *redirect, t_redirect **fds,  char *filename, char *fd)
{
    // NOTE: UPDATES fds based on the possible input redirection
    (*fds)->orig_fd = STDIN_FILENO;
    if (fd)
	(*fds)->orig_fd = ft_atoi(fd);
    (*fds)->new_fd = open(filename, O_RDONLY);
    if ((*fds)->new_fd == -1)
    {
	//TODO: strerror(errno) || bring pipex error writing here
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    if (!vec_push(redirect, *fds))
	return (0);
    return (1);
}

int ft_output_redir(t_vec *redirect, t_redirect **fds,  char *filename, char *fd)
{
    // NOTE: UPDATES fds based on the possible output redirection
    (*fds)->orig_fd = STDOUT_FILENO;
    if (fd)
	(*fds)->orig_fd = ft_atoi(fd);
    (*fds)->new_fd = open(filename, O_RDONLY | O_CREAT, 0644);
    if ((*fds)->new_fd == -1)
    {
	//TODO: strerror(errno) || bring pipex error writing here
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    if (!vec_push(redirect, *fds))
	return (0);
    return (1);
}

int ft_output_append(t_vec *redirect, t_redirect **fds,  char *filename, char *fd)
{
    // NOTE: UPDATES fds based on the possible output append redirection
    (*fds)->orig_fd = STDOUT_FILENO;
    if (fd)
	(*fds)->orig_fd = ft_atoi(fd);
    (*fds)->new_fd = open(filename, O_RDONLY | O_CREAT, 0644);
    if ((*fds)->orig_fd == -1)
    {
	//TODO: strerror(errno) || bring pipex error writing here
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    if (!vec_push(redirect, *fds))
	return (0);
    return (1);
}

int ft_here_doc(t_vec *redirect, t_redirect **fds,  char *eof, char *fd)
{
    // NOTE:UPDATES the fds and reads STDIN and saves the input to hidden file .here_doc 
    char *line;

    (*fds)->orig_fd = STDIN_FILENO;
    if (fd)
	(*fds)->orig_fd = ft_atoi(fd);
    (*fds)->new_fd = open(".infile.txt", O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if ((*fds)->orig_fd == -1)
    {
	//TODO: strerror(errno) || bring pipex error writing here
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    line = get_next_line(0);
    while (42)
    {
	if (((ft_strlen(line) - 1) == ft_strlen(eof) && !strncmp(line,
				eof, strlen(eof))) || !line)
		break ;
	write((*fds)->orig_fd, line, ft_strlen(line));
	line = get_next_line(0);
    }
    return (1);
}

int	ft_handle_redirects(t_input *input, char *s, char *fd, t_vars *vars)
{
    // NOTE: checks the string right after the redirect operator is not an invalid operator and if so returns 1
    // OTHERWISE tries to open the files and save their file descriptors and moves the pointer to the next string after filename
    size_t ind;
    char *file;
    t_vec redirect;
    t_redirect *fds;

    file = ft_next_string(vars, FILENAME);
    if (!file)// NOTE: what failed? malloc or syntax error?
	return (0);
    if (!vec_new(&redirect, 2, sizeof(t_redirect *)))// NOTE: Initialize a vector and allocate some mem for fds
	return (0);
    fds = (t_redirect *)malloc(sizeof(t_redirect));
    if (!fds)
	return (0); // WARNING: make sure to differentiate malloc fail or file open fail if needed be
    if (!ft_strncmp(s, "<<", 2))
    {
	if (!ft_here_doc(&redirect, &fds, file, fd))
	    return (0);
    }
    else if (!ft_strncmp(s, "<", 1))
    {
	if (!ft_input_redir(&redirect, &fds, file, fd))
	    return (0);
    }
    else if (!ft_strncmp(s, ">", 1))
    {
	if (!ft_output_redir(&redirect, &fds, file, fd))
	    return (0);
    }
    else if (!ft_strncmp(s, ">>", 2))
    {
	if (!ft_output_append(&redirect, &fds, file, fd))
	    return (0);
    }
    if (!vec_push(&redirect, fds))
	return (0);
    input->redirect = &redirect;
    free(file);
    return (1);
}

