
#include "minishell.h"

static	void	ft_stderr_redirection(t_pipex *pipex, int orig_fd, int index, int redir_type)
{
	int	new_fd;

	new_fd = *(int *)vec_get((*pipex->input)->new_fds, index);
	dup2(new_fd, orig_fd);
	if (redir_type == OUTPUT && orig_fd == STDERR_FILENO)
		pipex->err_to_pipe = NO;
}

static	void	ft_stdout_redirection(t_pipex *pipex, int orig_fd, int index, int redir_type)
{
	int	new_fd;

	new_fd = *(int *)vec_get((*pipex->input)->new_fds, index);
	dup2(new_fd, orig_fd);
	if (redir_type == OUTPUT && orig_fd == STDOUT_FILENO)
		pipex->output_to_pipe = NO;
}

static	void	ft_stdin_redirection(t_pipex *pipex, int orig_fd, int index)
{
	int	new_fd;

	new_fd = *(int *)vec_get((*pipex->input)->new_fds, index);
	dup2(new_fd, orig_fd);
	if (orig_fd == STDIN_FILENO)
		pipex->read_from_pipe = NO;
}

void	ft_io_redirections(t_pipex *pipex, int redir_type)
{
	int	ind;
	int	orig_fd;
	int	fds_info;

	ind = -1;
	orig_fd = 42;
	fds_info = 42;
	while ((*pipex->input)->new_fds && ++ind < (int)(*pipex->input)->new_fds->len)
	{
		fds_info = *(int *)vec_get((*pipex->input)->fds_info, ind);
		orig_fd = *(int *)vec_get((*pipex->input)->orig_fds, ind);
		if (fds_info == STDIN_FILENO)
			ft_stdin_redirection(pipex, orig_fd, ind);
		else if (fds_info == STDOUT_FILENO)
			ft_stdout_redirection(pipex, orig_fd, ind, redir_type);
		else if (fds_info == STDERR_FILENO)
			ft_stderr_redirection(pipex, orig_fd, ind, redir_type);
	}
}


