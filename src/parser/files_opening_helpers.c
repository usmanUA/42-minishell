
#include "minishell.h"

void	ft_free_redirect_strs(char **fd, char **redir, char **file);
int	ft_free_redirect_strs_prompt(char **fd, char **redir, char **file, t_shell *shell);

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
		return (ft_free_redirect_strs_prompt(shell->vars->f_des, shell->vars->redir, shell->vars->file, shell));
	if (!vec_push((*shell->input)->orig_fds, &redirect))
		return (ft_free_redirect_strs_prompt(shell->vars->f_des, shell->vars->redir, shell->vars->file, shell));
	// NOTE: fd_in is in stack mem of this function, be careful
	if (redirect == STDERR_FILENO)
		fd_info = STDERR_FILENO;
	if (!vec_push((*shell->input)->fds_info, &fd_info))
		return (ft_free_redirect_strs_prompt(shell->vars->f_des, shell->vars->redir, shell->vars->file, shell));
	// NOTE: fd_in is in stack mem of this function, be careful
	ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir, shell->vars->file);
	return (SUCCESS);
}

// TODO: check if decrementing redir_count with correct logic
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
		return (NOFILE);
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
	return (SUCCESS);
}

int	ft_open_here_doc(t_input **input, t_vars *vars)
{
	vars->file_fd = open(".here_doc", O_RDONLY);
	if (vars->file_fd == -1)
	{
		ft_filerror(errno, ".here_doc", NO);
		*(*input)->file_flag = BROWN;
		return (NOFILE);
		//WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
	}
	return (1);
}

