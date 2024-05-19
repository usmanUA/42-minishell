/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 13:07:37 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/15 15:06:26 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISELL_H
#define MINISELL_H

#include "macros.h"
#include "vec.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>

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
	int out_redir;
}	t_redir_count;

typedef struct s_vars
{
	char *input_line;
	int	ind;
	int	len;
	int end;
	int exit_status;
	int fd;
	int file_fd;
	int redirection_type;
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
	int redirections;
	char **f_des;
	char **file;
	char **redir;
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
	int	idx;
	int	cmd_flag;
	int	exec_type;
	int	tot_pipes;
	int	infile;
	int	read_end;
	int	write_end;
	int	read_from_pipe;
	int	output_to_pipe;
	int	err_to_pipe;
	char	*command;
	t_input		**input;
}			t_pipex;

typedef struct envp_list
{
    char                *key;
    char                *value;
    struct envp_list    *next;
} t_envp;

typedef struct s_shell
{
	int	status;
	char    **envp;
	t_input    **input;
	t_envp	*env_list;
	t_vec	*pipes;
	t_vec	*pids;
	t_vars	*vars;

} t_shell;


void	ft_init_vars(t_vars *vars);
int ft_space_until_end(t_vars *vars);
int ft_save_input(t_shell *shell);
int	ft_handle_redirects(t_shell *shell);
char	*ft_next_string(t_shell *shell, int op);
void	ft_shift_pointer(t_shell *shell);
void	ft_next_pipe_null(t_vars *vars);
int ft_operator_first(t_input *input, t_vars *vars);
int ft_command_first(t_input *input, t_vars *vars);
int	ft_save_cmd_filename(t_shell *shell, char **s, int op);
int	ft_save_cmd(t_shell *shell);
int ft_token_error(char c, int sgle);
void ft_index_after_spaces(t_vars *vars);
int ft_redirection(t_vars *vars);

void	ft_filerror(int errnu, char *filename, int write);
void	ft_cmd_error(char *cmd, int permission, int file_exist);

void	ft_print_vecs(t_vec *pipes);

void	ft_validate_commands(t_pipex *pipex, t_shell *shell);

void    ft_validate_execute(t_shell *shell);
int	ft_execute(t_shell *shell);
int	ft_processes(t_pipex *pipex, t_shell *shell);
int	ft_execute_last_cmd(t_pipex *pipex, t_shell *shell);

int ft_init_shell(t_shell *shell);
void	ft_init_vars(t_vars *vars);
int	ft_init_redirect_vecs(t_shell *shell, t_redir_count *redir_count);
void	ft_init_pipex(t_pipex *pipex, int tot_pipes);


int ft_syntax_error(t_vars *vars);
int ft_prohibited_chars(t_vars *vars);

void	ft_signals(int place, int button, int *status);

int	ft_valid_input(t_vars *vars, t_shell *shell);
int ft_prompt(t_shell *shell);

void	ft_listadd_back(t_envp **lst, t_envp *new_list);
char	*get_name_of_env_variable(t_shell *data, char *envp_string);
char	*get_value_of_env_variable(t_shell *data, char *envp_string);
int	make_linked_list_of_envp(t_shell *data, char **envp);
t_envp	*create_new_node(t_shell *data);

int	ft_array_length(char **envp);;

void	error_msg_hardcode(char *command, char *argument, int error_number, bool quotes);
void	error_msg_2(int error_number);

// NOTE: builtin functions prototypes
int		cd_command(t_shell *data, char **command);
int		echo_command(char **command);
int		env_command(t_shell *data, char **command);
int	builtin_commands(t_shell *shell, char **command, int exec_type);
int	exit_command(t_shell *data, char **command);
int		export_command(t_shell *data, char **command);
int	ft_free_prompt(t_shell *shell, int input_separate);
void	free_env_list(t_shell *data);
int		pwd_command(t_shell *data);
t_envp	*search_for_envp(t_shell *data, char *command);
int		unset_command(t_shell *data, char **command);

void	ft_free_input(void **inpt);
void	free_env_array(t_shell *data);

void	ft_skip_quotes(t_vars *vars);
int		ft_strcmp(char *s1, char *s2);
char	*string_n_copy(char *dest, char *source, int len);
char	**malloc_envp(t_shell *shell, char **envp);
#endif