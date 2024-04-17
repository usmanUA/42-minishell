/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 10:49:17 by uahmed            #+#    #+#             */
/*   Updated: 2024/03/15 16:29:11 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "errno.h"
# include "fcntl.h"
# include "libft/libft.h"
# include "stdio.h"
# include "string.h"
# include "sys/wait.h"
# include "unistd.h"

typedef enum e_bool
{
	false,
	true
}			t_bool;

typedef struct s_pipex
{
	int		fd_in;
	int		fdin_failed;
	int		fd_out;
	int		fdout_failed;
	int		fdout_errno;
	int		start;
	int		idx;
	t_bool	here_doc;
	char	*infile;
	char	*outfile;
	char	**paths;
	char	**cmds;
	char	***cmd_args;
	int		tot_cmds;
	int		*pids;
	int		*fds;
}			t_pipex;

void		ft_initialize_pipex(t_pipex *pipex, int tot_cmds, char *infile,
				char *outfile);

void		ft_validate_files(char *infile, char *outfile, t_pipex *pipex);

char		**ft_paths(char *cmds[], char **envp, int tot_cmds);
char		*ft_give_path(char **envp);
char		*ft_join_path(char *path, t_pipex *pipex);
void		ft_validate_commands(char **args, char **envp, t_pipex *pipex);
void		ft_make_command(t_pipex *ppx);
void		ft_handle_absolute(t_pipex *ppx);
void		ft_handle_relative(t_pipex *ppx);

void		ft_filerror(int errnu, t_pipex *pipex, int infile, int write);
void		ft_cmd_error(char *cmd, int permission, int file_exist);
void		ft_exit_error(t_pipex *pipex, int free, int status, int wait_index);
int			ft_notwrite_cmderror(t_pipex *ppx);

int			ft_execute(t_pipex *pipex, char **envp);
int			ft_pair_failed(t_pipex *pipex, int cmd_num, int i);
int			ft_status(int status);
void		ft_wait_pids(t_pipex *ppx, int index);

void		ft_free_pipex(t_pipex *pipex);

#endif
