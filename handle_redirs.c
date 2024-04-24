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

int ft_input_redir(t_redirect *fds,  char *filename, char *fd)
{
    // NOTE: UPDATES fds based on the possible input redirection
    fds->orig_fd = STDIN_FILENO;
    if (fd)
	fds->orig_fd = ft_atoi(fd);
    fds->new_fd = open(filename, O_RDONLY);
    if (fds->new_fd == -1)
    {
	//TODO: strerror(errno) || bring pipex error writing here
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    return (1);
}

int ft_output_redir(t_redirect *fds,  char *filename, char *fd)
{
    // NOTE: UPDATES fds based on the possible output redirection
    fds->orig_fd = STDOUT_FILENO;
    if (fd)
	fds->orig_fd = ft_atoi(fd);
    fds->new_fd = open(filename, O_RDONLY | O_CREAT, 0644);
    if (fds->new_fd == -1)
    {
	//TODO: strerror(errno) || bring pipex error writing here
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    return (1);
}

int ft_output_append(t_redirect *fds,  char *filename, char *fd)
{
    // NOTE: UPDATES fds based on the possible output append redirection
    fds->orig_fd = STDOUT_FILENO;
    if (fd)
	fds->orig_fd = ft_atoi(fd);
    fds->new_fd = open(filename, O_RDONLY | O_CREAT, 0644);
    if (fds->orig_fd == -1)
    {
	//TODO: strerror(errno) || bring pipex error writing here
	return (0); //WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
    }
    return (1);
}

int ft_here_doc(t_redirect *fds,  char *eof, char *fd)
{
    // NOTE:UPDATES the fds and reads STDIN and saves the input to hidden file .here_doc 
    char *line;

    fds->orig_fd = STDIN_FILENO;
    if (fd)
	fds->orig_fd = ft_atoi(fd);
    fds->new_fd = open(".infile.txt", O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (fds->orig_fd == -1)
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
	write(fds->orig_fd, line, ft_strlen(line));
	line = get_next_line(0);
    }
    return (1);
}

int	ft_handle_redirects(t_vec *redirect, t_vars *vars)
{
    // NOTE: checks the string right after the redirect operator is not an invalid operator and if so returns 1
    // OTHERWISE tries to open the files and save their file descriptors and moves the pointer to the next string after filename
    size_t ind;
    char *s;
    char *fd;
    char *file;
    t_redirect *fds;

    if (vars->fd)
    {
	fd = ft_next_string(vars, FD); // NOTE: str in heap, vars->ind points ->FD)<-vars->end 
	if (!fd)
	    return (0); // NOTE: malloc fails
    }
    vars->ind = vars->end;
    s = ft_next_string(vars, REDIRECT); // NOTE: str in heap, vars->ind points ->opertr)<-vars->end 
    if (!s)
	return (0); ; // NOTE: malloc fails (am I handling it in a correct way?)
    // TODO: check the cases -> 0;, 0| (cases where fd is followed by special chars other than redirects)
    if (ft_special_char(vars->input_line[vars->end]))
    {
	ft_shift_pointer(vars); // NOTE: vars->ind points to either \0 or the character followed by |
	ft_token_error(vars->input_line[vars->end]);
	vars->end = vars->ind;// NOTE: vars->end points to either \0 or the character followed by |
	free(s);
	free(fd);
	return (0); // WARN: NO malloc fail but unexprected token error
    }
    file = ft_next_string(vars, FILENAME);
    if (!file)
	return (0); // NOTE: malloc fail
    fds = (t_redirect *)malloc(sizeof(t_redirect));
    if (!fds)
	return (0);  // NOTE: malloc fail
    // WARNING: make sure to differentiate malloc fail or file open fail if needed be
    if (!ft_strncmp(s, "<<", 2))
    {
	if (!ft_here_doc(fds, file, fd))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    else if (!ft_strncmp(s, "<", 1))
    {
	if (!ft_input_redir(fds, file, fd))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    else if (!ft_strncmp(s, ">", 1))
    {
	if (!ft_output_redir(fds, file, fd))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    else if (!ft_strncmp(s, ">>", 2))
    {
	if (!ft_output_append(fds, file, fd))
	    return (0); // NOTE: could be malloc error or file opening error
    }
    if (!vec_push(redirect, fds))
	return (0); // NOTE: malloc fail
    free(file);
    return (1);
}

