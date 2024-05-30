/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 13:07:37 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/24 13:57:13 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "macros.h"
# include "vec.h"
# include <fcntl.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/errno.h>
# include <termios.h>
# include <unistd.h>

extern int				g_signal_status;

typedef struct s_redirect
{
	t_vec				*new_fds;
	t_vec				*orig_fds;
	t_vec				*fds_info;
	int					**file_flag;
}						t_redirect;

typedef struct s_redir_count
{
	int					in_redir;
	int					out_redir;
}						t_redir_count;

typedef struct s_vars
{
	char				*input_line;
	int					ind;
	int					len;
	int					end;
	int					exit_status;
	int					heredoc;
	int					fd;
	int					file_fd;
	int					redirection_type;
	int					qontinue;
	int					expand_it;
	int					expanded;
	int					malloc_flag;
	int					s_quote;
	int					d_quote;
	int					increment;
	int					stop;
	int					file_error;
	int					unlink_here_doc;
	int					redirections;
	char				**f_des;
	char				**file;
	char				**redir;
	t_redir_count		*redir_count;
	t_redirect			*redirect;
}						t_vars;

typedef struct s_input
{
	t_vec				*cmd;
	t_vec				*new_fds;
	t_vec				*orig_fds;
	t_vec				*fds_info;
	int					*file_flag;
}						t_input;

typedef struct s_pipex
{
	int					idx;
	int					cmd_flag;
	int					exec_type;
	int					tot_pipes;
	int					infile;
	int					read_end;
	int					write_end;
	int					read_from_pipe;
	int					output_to_pipe;
	int					err_to_pipe;
	char				*command;
	t_input				**input;
}						t_pipex;

typedef struct envp_list
{
	char				*key;
	char				*value;
	struct envp_list	*next;
}						t_envp;

typedef struct s_shell
{
	int					exit;
	int					status;
	char				**envp;
	t_input				**input;
	t_envp				*env_list;
	t_vec				*pipes;
	t_vec				*pids;
	t_vars				*vars;

}						t_shell;

/*********************	builtins *********************************/

int						builtin_commands(t_shell *shell, char **command,
							int exec_type);
int						cd_command(t_shell *shell, char **command);
int						change_oldpwd(t_shell *shell);
int						change_oldpwd_and_pwd(t_shell *shell);
int						check_if_number_greater_than_long_max(
							char *exit_argument);
int						check_if_number_smaller_than_long_min(
							char *exit_argument);
int						check_if_too_many_arguments(t_shell *shell,
							char **command);
char					*create_empty_string(void);
void					delete_environmental_variable(t_shell *shell,
							char *command);
int						echo_command(char **command);
int						env_command(t_shell *shell, char **command);
int						exit_command(t_shell *shell, char **command);
int						export_command(t_shell *shell, char **command);
bool					export_with_arguments(t_shell *shell, char **command,
							bool error_flag);
int						ft_return_value(int error_flag);
int						ft_strcmp(char *s1, char *s2);
char					*get_name_of_env_variable(char *envp_string);
char					*get_value_of_env_variable(char *envp_string);
int						if_env_var_exists(t_shell *shell, char *command);
int						pwd_command(void);
char					*relative_path(t_shell *shell, char *command);
int						return_index_of_node_above_unset_node(t_shell *shell,
							t_envp *unset_node);
t_envp					*search_for_envp(t_shell *shell, char *command);
int						unset_command(t_shell *shell, char **command);
int						update_2d_env_array(t_shell *shell);

/*********************	error *********************************/

void					error_msg(char *command, char *argument,
							int error_number, bool quotes);
void					error_msg_2(int error_number);
void					ft_cmd_error(char *cmd, int permission, int file_exist);
void					ft_filerror(int errnu, char *filename, int write);
int						ft_token_error(char c, int sgle);

/*********************	exec *********************************/

int						ft_execute_last_cmd(t_pipex *pipex, t_shell *shell);
int						ft_processes(t_pipex *pipex, t_shell *shell);
void					ft_validate_commands(t_pipex *pipex, t_shell *shell);
void					ft_validate_execute(t_shell *shell);

/*********************	free *********************************/

void					deallocate_all_envps(t_shell *shell);
void					free_env_array(t_shell *shell);
void					free_env_list(t_shell *shell);
int						free_failed_2d_array(t_shell *shell, char **envp,
							int i);
int						ft_free_prompt(t_shell *shell, int input_separate);

/*********************	init *********************************/

void					allocate_all_envps(t_shell *shell, char **envp,
							int argc, char **argv);
t_envp					*create_new_node(void);
void					increment_2d_shell_level(t_shell *shell, char **envp);
void					ft_init_pipex(t_pipex *pipex, int tot_pipes);
int						ft_init_redirect_vecs(t_shell *shell,
							t_redir_count *redir_count);
int						ft_init_shell(t_shell *shell);
void					ft_init_vars(t_vars *vars);
void					ft_listadd_back(t_envp **lst, t_envp *new_list);
t_envp					*ft_listlast(t_envp *lst);
char					**malloc_envp(t_shell *shell, char **envp);

/*********************	main *********************************/

int						ft_prompt(t_shell *shell);
void					ft_signals(int place, int button);
int						ft_valid_input(t_vars *vars, t_shell *shell);

/*********************	parser *********************************/

int						ft_handle_redirects(t_shell *shell);
void					ft_index_after_spaces(t_vars *vars);
char					*ft_next_string(t_shell *shell, int op);
int						ft_redirection(t_vars *vars);
int						ft_save_cmd(t_shell *shell);
int						ft_save_cmd_filename(t_shell *shell, char **s, int op);
int						ft_save_input(t_shell *shell);
void					ft_shift_pointer(t_shell *shell);
void					ft_skip_quotes(t_vars *vars);

/*********************	syntax_checker *********************************/

int						ft_prohibited_chars(t_vars *vars);
int						ft_syntax_error(t_vars *vars);

#endif
