/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_user.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:55:03 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/16 13:55:05 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft/libft.h"
#include "miniwell.h"

void	ft_init_vars(t_vars *vars)
{
    vars->ind = -1;
    vars->end = 0;
    vars->begin = 1;
    vars->len = 0;
    vars->input_line = NULL;
}

static size_t	ft_user_ends(char *s)
{
    while (*s)
    {
	if (!ft_isspace(*s))
	    return (0);
	s++;
    }
    return (1);
}

void	ft_strings_end(t_vars *vars)
{
    int ind;
    int	quote;

    quote = 0;
    ind = 0;
    if (vars->input_line[vars->ind] == '\'' || vars->input_line[vars->ind])
	quote = 1;
    while (vars->input_line[vars->ind + ind] && !ft_isspace(vars->input_line[vars->ind + ind]))
    {
	while (quote && ft_isspace(vars->input_line[vars->ind+ind]))
	    ++ind;
	++ind;
    }
    vars->end = vars->ind + ind;
}


int ft_index_after_spaces(t_vars *vars)
{
    int ind;

    ind = 0;
    if (ft_space_until_end(&vars->input_line[vars->end]))
	return (0);
    while (ft_isspace(vars->input_line[vars->end+ind]))
	ind++;
    vars->ind = vars->end + ind;
    return (1);
}

static int ft_redirect_operator(const char *str)
{
   if (!ft_strncmp(str, "<", 1) || !ft_strncmp(str, ">", 1))
	return (1);
    if (!ft_strncmp(str, "<<", 2) || !ft_strncmp(str, ">>", 2))
	return (1);
    return (0);
}

void	ft_shift_pointer(t_vars *vars)
{
    while (vars->end < vars->len)
    {
	if (vars->input_line[vars->end] == '|' || vars->input_line[vars->end] == '\0') // NOTE: if encounters | or '\0' stops.
	    return ;
	vars->end++;
    }
}

int ft_save_input(t_vec *pipes, t_vars *vars)
{
    t_vec cmd;
    t_vec redirect;
    char *str;

    if (!vec_new(&cmd, 2, sizeof(char *))) // NOTE: Initialize a vector and allocate some mem for command
	return (0);
    if (!vec_new(&redirect, 2, sizeof(t_redirect *)))// NOTE: Initialize a vector and allocate some mem for fds
	return (0);
    while (++vars->ind < vars->len) // NOTE: loop over the whole user input line
    {
	if (!ft_index_after_spaces(vars)) // NOTE: skip spaces, returns the index of the string or 0 if the whole line is spaces and now word/string 
	    break;
	ft_strings_end(vars); // NOTE: index of the string before the next space or null-terminator starts
	str = ft_substr(&vars->input_line[vars->ind], vars->ind, vars->end - vars->ind); // NOTE: malloc that string in heap and point str to it
	if (vars->begin && ft_redirect_operator(str)) // NOTE: check if the first string is one of the redir operators
	{
	    if (!ft_save_redirects(&redirect, str, vars)) // NOTE: open files and save their fds to vec. operators are followed by files 
		return (0);
	}
	else if (!ft_strncmp(str, "|", 1))
	{
	    if (ft_syntax_error(vars, vars->len - vars->end)) // NOTE: if there's syntax error after the |
		ft_shift_pointer(vars); // NOTE: move to the end or to the next |
	    vars->begin = 1;
	}
	else
	    vec_push(&cmd, str); // NOTE: if its command or its option, save to the vec.
    }
    input->cmd = &cmd;
    input->redirect = &redirect;
    return (1);
}
