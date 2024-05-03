/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniwell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 13:07:37 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/15 13:07:39 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MINIWELL_H
#define MINIWELL_H

#include "vec/vec.h"
#include "libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <readline/readline.h>
# include "get_next_line/get_next_line.h"

typedef enum s_flags
{
	NOFLAG,
	REDIR,
	PIPE,
	GREEN,
	YELLOW,
	RED,
	YES,
	NO,
	VALID,
	INVALID,
	MALLOC_FAIL,
	MALLOC_SUCCESS,
}	t_flags;

typedef enum s_consants
{
	COMMAND = 0,
	REDIRECT = 1,
	FD = 2,
	FILENAME = 3,
}	t_constants;
 
typedef enum s_redir_types
{
	INFILE = 0,
	OUTFILE = 1,
	APPEND = 2,
	HERE_DOC = 3,
}	t_redir_types;

typedef struct s_redirect
{
	t_vec *new_fds;
	t_vec *orig_fds;
	t_vec *fds_info;
	int **file_flag;
}	t_redirect;

typedef struct s_redir_count
{
	int in_redir;
	int err_redir;
	int out_redir;
}	t_redir_count;

typedef struct s_vars
{
	char *input_line;
	size_t	ind;
	size_t	len;
	size_t end;
	int fd;
	int qontinue;
	int expand_it;
	int expanded;
	int malloc_flag;
	int s_quote;
	int d_quote;
	int increment;
	int stop;
	int file_error;
	int unlink_here_doc;
	t_redir_count *redir_count;
	t_redirect *redirect;
}	t_vars;

typedef struct s_input
{
	t_vec *cmd;
	t_vec *new_fds;
	t_vec *orig_fds;
	t_vec *fds_info;
	int *file_flag;
}	t_input;

typedef struct s_pipex
{
	int		idx;
	int		infile;
	int		status;
	int		cmd_flag;
	int		*fds;
	t_vec		*pids;
	t_input		*input;
}			t_pipex;

typedef struct envp_list
{
    char                *env_name;
    char                *env_value;
    struct envp_list    *next;
} t_envp;

typedef struct s_shell
{
	char    **envp;
	t_envp    *envp_linkedlist;
	t_vec	*pipes;
	t_vec	*info;
	t_vars	*vars;

} t_shell;

void	ft_init_vars(t_vars *vars);
int ft_syntax_error(t_vars *vars);
int ft_space_until_end(t_vars *vars);
int ft_save_input(t_shell *shell);
int	ft_handle_redirects(t_input **input, t_vars *vars, t_envp *env_vars);
char *ft_next_string(t_vars *vars, int op, t_envp *env_vars);
void	ft_shift_pointer(t_vars *vars);
void	ft_next_pipe_null(t_vars *vars);
int ft_operator_first(t_input *input, t_vars *vars);
int ft_command_first(t_input *input, t_vars *vars);
int ft_save_cmd_filename(t_vars *vars, char **s, t_envp *env_vars, int op);
int ft_save_cmd(t_vec *cmd, t_vars *vars, t_envp *env_vars);
int ft_token_error(char c, int sgle);
void ft_index_after_spaces(t_vars *vars);
int ft_redirection(t_vars *vars);
void	ft_free_vec(t_shell *shell);

void	ft_filerror(int errnu, char *filename, int write);
void	ft_cmd_error(char *cmd, int permission, int file_exist);

void	ft_print_vecs(t_vec *pipes);

int	ft_validate_commands(t_input *input, t_vec *info, char **envp);

int    ft_validate_execute(t_shell *shell);
int	ft_execute(t_shell *shell);
void	ft_processes(t_input *input, t_pipex *pipex, char **envp);
int	ft_execute_last_cmd(t_input *input, t_pipex *pipex, char **envp);

int ft_init_shell(t_shell *shell, char **envp);
void	ft_init_vars(t_vars *vars);
int ft_init_redirect_vecs(t_input **input, t_redir_count *redir_count);
void    ft_init_pipex(t_pipex *pipex);

int    make_linked_list_of_envp(t_shell *data, char **envp);
void    print_envp_list_instance(t_shell *data);

#endif
