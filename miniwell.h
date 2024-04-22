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
#include <errno.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <sys/_types/_size_t.h>
# include "get_next_line/get_next_line.h"

typedef enum s_constants
{
	COMMAND = 0,
	REDIRECT = 1,
	FD = 2,
	FILENAME = 3,
}	t_constants;
 
typedef struct s_vars
{
	char *input_line;
	size_t	ind;
	size_t	len;
	size_t end;
	int fd;
	int s_quote;
	int d_quote;
}	t_vars;

typedef struct s_pipes
{
	t_vec *pipes;
	t_vec *pos;
}	t_pipes;

typedef struct s_input
{
	t_vec *cmd;
	t_vec *redirect;
}	t_input;

typedef struct s_redirect
{
	int orig_fd;
	int new_fd;
}	t_redirect;

void	ft_init_vars(t_vars *vars);
int ft_syntax_error(t_vars *vars);
int ft_space_until_end(char *s);
int ft_save_input(t_vec *pipes, t_vars *vars);
int	ft_handle_redirects(t_input *input, char *s, char *fd, t_vars *vars);
char *ft_next_string(t_vars *vars, int op);
void	ft_shift_pointer(t_vars *vars);
void	ft_next_pipe_null(t_vars *vars);
int ft_special_char(char s);
int ft_operator_first(t_input *input, t_vars *vars);
int ft_command_first(t_input *input, char *s, t_vars *vars);
int ft_save_cmd(t_input *input, char *s, t_vars *vars);
int ft_token_error(char c);

#endif
