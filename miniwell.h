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

 
typedef struct s_vars
{
	char *input_line;
	size_t	ind;
	size_t	len;
	size_t end;
	int	begin;
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
int ft_syntax_error(t_vars *vars, size_t len);
int ft_space_until_end(char *s);
int ft_save_input(t_vec *pipes, t_vars *vars);
int	ft_save_redirects(t_input *input, const char *s, t_vars *vars);
void	ft_strings_end(t_vars *vars);
int ft_index_after_spaces(t_vars *vars);

#endif
