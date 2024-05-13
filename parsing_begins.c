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
#include "minishell.h"
#include <cstdio>
#include <cstdlib>

void	ft_index_after_spaces(t_vars *vars)
{
	// NOTE: SKIPS all the spaces and moves the pointer to the non-space character
	while (vars->input_line[vars->end]
		&& ft_isspace(vars->input_line[vars->end]))
		vars->end++;
	vars->ind = vars->end; // NOTE: index updates here
	//                    '\0'
}

// NOTE: NOT USED function
void	ft_shift_pointer(t_vars *vars)
{
	// NOTE: moves the pointer either to the next | or null-terminator whatever comes first in the char *
	vars->ind = vars->end;
	while (vars->ind < vars->len)
	{
		if (vars->input_line[vars->ind] == '|') // NOTE: if encounters | stops.
		{
			vars->ind++;
			return ;
		}
		vars->ind++;
	}
}

int	ft_redirection(t_vars *vars)
{
	int	ind;

	// NOTE: checks the operator-first case
	ind = vars->ind;
	if (vars->input_line[ind] == '<' || vars->input_line[ind] == '>')
		return (YES);
	while (ft_isdigit(vars->input_line[ind]))
		ind++;
	if (vars->input_line[ind] == '<' || vars->input_line[ind] == '>')
	{
		vars->fd = 1;
		return (YES);
	}
	return (NO);
}

static int	ft_parsing_action(t_input **input, t_vec *cmd, t_shell *shell)
{
	char	c;

	// NOTE: Loops until either '\0' or '|'
	// parses everything in between
	// moves the pointer next to '|' if encountered
	c = shell->vars->input_line[shell->vars->ind];
	while (c != '\0' && c != '|')
	{
		if (ft_redirection(shell->vars) == YES)
		{
			if (!ft_handle_redirects(input, shell->vars,
					shell->env_list)) // TODO: look for error handling
				return (MALLOC_FAIL); // TODO: differentiate malloc_fail with file_failure, especially here_doc
		}
		else if (c != '\0' && c != '|')
		{
			if (!ft_save_cmd(cmd, shell->vars, shell->env_list))
			// TODO: look for error handling
				return (MALLOC_FAIL);
		}
		ft_index_after_spaces(shell->vars);
		c = shell->vars->input_line[shell->vars->ind];
	}
	if (shell->vars->input_line[shell->vars->ind] == '|')
	{
		shell->vars->end++;
		ft_index_after_spaces(shell->vars);
	}
	return (MALLOC_SUCCESS);
}

static int	ft_parse_command_line(t_input **input, t_shell *shell)
{
	int		*file_flag;
	t_vec	*cmd;
	char	*null;

	// NOTE: PARSES the WHOLE command line into the following two vectors
	null = NULL;
	cmd = (t_vec *)malloc(sizeof(t_vec));
	if (!cmd)
		return (MALLOC_FAIL);
	if (!vec_new(cmd, 2, sizeof(char **)))
	// NOTE: Init a vec and allocate some mem for command
		return (MALLOC_FAIL);              // NOTE: malloc fail
	file_flag = (int *)malloc(sizeof(int));
	if (!file_flag)
		return (MALLOC_FAIL);
	*file_flag = GREEN;
	(*input)->file_flag = file_flag;
	if (ft_parsing_action(input, cmd, shell) == MALLOC_FAIL) // TODO: Handle errors correctly
		return (MALLOC_FAIL);                           
		// NOTE: other error than malloc fail?
	if (!vec_push(cmd, &null))
		return (MALLOC_FAIL); // NOTE: malloc fail
	// NOTE: saving addresses of cmd and redirect to input
	(*input)->cmd = cmd;
	return (MALLOC_SUCCESS);
}

static void	ft_update_index(int redirect, int *ind, int count)
{
	int	no_use;

	no_use = redirect;
	*ind += count;
}

void	ft_count_redirs(t_vars *vars, t_redir_count *redir_count)
{
	int		ind;
	char	c;

	ind = vars->ind;
	c = vars->input_line[ind];
	while (c != '\0' && c != '|')
	{
		if (!ft_strncmp(&vars->input_line[ind], "<<", 2))
			ft_update_index(++redir_count->in_redir, &ind, 2);
		else if (!ft_strncmp(&vars->input_line[ind], ">>", 2))
			ft_update_index(++redir_count->out_redir, &ind, 2);
		else if (!ft_strncmp(&vars->input_line[ind], "<", 1))
			ft_update_index(++redir_count->in_redir, &ind, 1);
		else if (!ft_strncmp(&vars->input_line[ind], ">", 1))
			ft_update_index(++redir_count->out_redir, &ind, 1);
		else
			ind++;
		c = vars->input_line[ind];
	}
}

void	ft_zero_redirects(t_redir_count *redir_count)
{
	redir_count->out_redir = 0;
	redir_count->in_redir = 0;
}

void	ft_free_parsed(t_input **input, t_vars *vars)
{
	if (vars->input_line)
		free(vars->input_line);
	if (*input)
	{
		ft_free_redirect(&(*input)->new_fds);
		ft_free_redirect(&(*input)->orig_fds);
		ft_free_redirect(&(*input)->fds_info);
		free(*input);
	}
}

int	ft_save_input(t_shell *shell)
{
	t_input			*input;
	t_redir_count	redir_count;

	// NOTE: PARSE EVERYTHING
	ft_zero_redirects(&redir_count);
	ft_index_after_spaces(shell->vars);
	while (shell->vars->input_line[shell->vars->ind] != '\0')
	// NOTE: loop over the whole user input line
	{
		ft_count_redirs(shell->vars, &redir_count);
		shell->vars->redir_count = &redir_count;
		input = malloc(sizeof(t_input));
			// NOTE: executed in the very beg. or the beg. of every pipe (|) if any
		if (!input)
		{
			ft_free_parsed(&input, shell->vars);
			exit(EXIT_FAILURE);
		}
		ft_init_redirect_vecs(&input, &redir_count); // TODO: FREE vars->input_line if malloc fails
		if (ft_parse_command_line(&input, shell) == MALLOC_FAIL)
		{
			ft_free_parsed(&input, shell->vars);
			exit(EXIT_FAILURE);
		}
		if (!vec_push(shell->pipes, &input))
		{
			ft_free_parsed(&input, shell->vars);
			exit(EXIT_FAILURE);
		}
		ft_zero_redirects(&redir_count);
	}
	return (MALLOC_SUCCESS);
}
