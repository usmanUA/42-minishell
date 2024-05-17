
#include "minishell.h"

int	ft_push_fds(t_shell *shell);
int	ft_get_here_doc(t_shell *shell);
int	ft_open_here_doc(t_input **input, t_vars *vars);
void	ft_free_redirect_strs(char **fd, char **redir, char **file);

static	int	ft_here_doc(t_shell *shell)
{
	int	flag;
	// NOTE:UPDATES the fds and reads STDIN and saves the input to hidden file .here_doc
	shell->vars->redirection_type = STDIN_FILENO;
	flag = ft_get_here_doc(shell);
	if (flag != MALLOC_SUCCESS)
	{
		ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir, shell->vars->file);
		return (flag);
	}
	if (shell->vars->redir_count->in_redir == 1)
	{
		if (ft_open_here_doc(shell->input, shell->vars) == FILE_FAIL)
		{
			ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir, shell->vars->file);
			return (FILE_FAIL); 
		}
		shell->vars->unlink_here_doc = YES;
		return (ft_push_fds(shell));
	}
	else if (shell->vars->redir_count->in_redir > 1)
	{
		unlink(".here_doc");
		shell->vars->redir_count->in_redir--;
	}
	ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir, shell->vars->file);
	return (MALLOC_SUCCESS);
}

static	int	ft_input_redir(t_shell *shell)
{
	// NOTE: UPDATES redirect based on the possible input redirection
	shell->vars->redirection_type = STDIN_FILENO;
	shell->vars->file_fd = open(*shell->vars->file, O_RDONLY);
	if (shell->vars->file_fd == -1)
	{
		ft_filerror(errno, *shell->vars->file, YES);
		*(*shell->input)->file_flag = BROWN;
		ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir, shell->vars->file);
		ft_shift_pointer(shell);
		return (FILE_FAIL);
		//WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
	}
	if (shell->vars->redir_count->in_redir == 1)
		return (ft_push_fds(shell));
	else if (shell->vars->redir_count->in_redir > 1)
		shell->vars->redir_count->in_redir--;
	ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir, shell->vars->file);
	return (MALLOC_SUCCESS);
}

static	int	ft_output_redir(t_shell *shell)
{
	// NOTE: UPDATES fds based on the possible output redirection
	shell->vars->redirection_type = STDOUT_FILENO;
	shell->vars->file_fd = open(*shell->vars->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (shell->vars->file_fd == -1)
	{
		ft_filerror(errno, *shell->vars->file, YES);
			// NOTE: write error when there's no permission for the output file
		*(*shell->input)->file_flag = BROWN;
		ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir, shell->vars->file);
		ft_shift_pointer(shell);
		return (FILE_FAIL);
		//WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
	}
	if (shell->vars->redir_count->out_redir == 1)
		return (ft_push_fds(shell));
	else if (shell->vars->redir_count->out_redir > 1)
		shell->vars->redir_count->out_redir--;
	ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir, shell->vars->file);
	return (MALLOC_SUCCESS);
}

static	int	ft_output_append(t_shell *shell)
{
	// NOTE: UPDATES fds based on the possible output append redirection
	shell->vars->redirection_type = STDOUT_FILENO;
	shell->vars->file_fd = open(*shell->vars->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (shell->vars->file_fd == -1)
	{
		ft_filerror(errno, *shell->vars->file, YES);
		*(*shell->input)->file_flag = BROWN;
		ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir, shell->vars->file);
		ft_shift_pointer(shell);
		return (FILE_FAIL);
		//WARN: how to differentiate btw malloc fail and file errors when returning 0 in both cases?
	}
	if (shell->vars->redir_count->out_redir == 1)
		return (ft_push_fds(shell));
	else if (shell->vars->redir_count->out_redir > 1)
		shell->vars->redir_count->out_redir--;
	ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir, shell->vars->file);
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
	ft_free_redirect_strs(shell->vars->f_des, shell->vars->redir, shell->vars->file);
	return (MALLOC_SUCCESS);
}

