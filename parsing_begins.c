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
void	ft_shift_pointer(t_shell *shell)
{
	char	*input_line;
	int	ind;
	// NOTE: moves the pointer either to the next | or null-terminator whatever comes first in the char *
	ind = shell->vars->end;
	input_line = shell->vars->input_line;
	while (input_line[ind] != '\0' && input_line[ind] != '|')
		ind++;
	shell->vars->ind = ind;
	shell->vars->end = ind;
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

void	ft_skip_special_expansions(t_shell *shell)
{
	char	next;

	next = shell->vars->input_line[shell->vars->ind+1];
	if (ft_isdigit(next) || next == '-')
	{
		shell->vars->ind += 2;
		shell->vars->end += 2;
	}
	else if (next == '\"' || next == '\'')
	{
		++shell->vars->ind;
		++shell->vars->end;
	}
	else
		return;
	ft_index_after_spaces(shell->vars);
}

static int	ft_parsing_action(t_shell *shell)
{
	char	c;
	char	next;

	// NOTE: Loops until either '\0' or '|'
	// parses everything in between
	// moves the pointer next to '|' if encountered
	c = shell->vars->input_line[shell->vars->ind];
	while (c != '\0' && c != '|')
	{
		if (c == '$')
			ft_skip_special_expansions(shell);
		if (ft_redirection(shell->vars) == YES)
		{
			if (ft_handle_redirects(shell) == MALLOC_FAIL)
				return (MALLOC_FAIL);
		}
		else if (c != '\0' && c != '|')
		{
			if (ft_save_cmd(shell) == MALLOC_FAIL)
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

static int	ft_parse_command_line(t_shell *shell)
{
	int		*file_flag;
	t_vec	*cmd;
	char	*null;
	t_input	**input;

	// NOTE: PARSES the WHOLE command line into the following two vectors
	null = NULL;
	input = shell->input;
	cmd = (t_vec *)malloc(sizeof(t_vec));
	if (!cmd)
		return (ft_free_prompt(shell, YES));;
	(*input)->cmd = cmd;
	vec_new(cmd, 0, sizeof(char **));
	file_flag = (int *)malloc(sizeof(int));
	if (!file_flag)
		return (ft_free_prompt(shell, YES));;
	*file_flag = GREEN;
	(*input)->file_flag = file_flag;
	if (ft_parsing_action(shell) == MALLOC_FAIL)
		return (MALLOC_FAIL);
	if (!vec_push(cmd, &null))
		return (ft_free_prompt(shell, YES));;
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
			return (ft_free_prompt(shell, NO));
		shell->input = &input;
		if (ft_init_redirect_vecs(shell, &redir_count) == MALLOC_FAIL) // TODO: FREE vars->input_line if malloc fails
			return (MALLOC_FAIL);
		if (ft_parse_command_line(shell) == MALLOC_FAIL)
			return (MALLOC_FAIL);
		if (!vec_push(shell->pipes, &input))
			return (ft_free_prompt(shell, YES));
		ft_zero_redirects(&redir_count);
	}
	return (MALLOC_SUCCESS);
}
