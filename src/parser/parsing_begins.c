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

int	ft_redirection(t_vars *vars);
void	ft_count_redirs(t_vars *vars, t_redir_count *redir_count);
void	ft_zero_redirects(t_redir_count *redir_count);
void	ft_skip_special_expansions(t_shell *shell);

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

void	ft_skip_pipe(t_shell *shell)
{
	if (shell->vars->input_line[shell->vars->ind] == '|')
	{
		shell->vars->end++;
		ft_index_after_spaces(shell->vars);
	}
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
			if (ft_handle_redirects(shell) == FAILURE)
				return (FAILURE);
		}
		else if (c != '\0' && c != '|')
		{
			if (ft_save_cmd(shell) == FAILURE)
				return (FAILURE);
		}
		ft_index_after_spaces(shell->vars);
		c = shell->vars->input_line[shell->vars->ind];
	}
	ft_skip_pipe(shell);
	return (SUCCESS);
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
	if (ft_parsing_action(shell) == FAILURE)
		return (FAILURE);
	if (!vec_push(cmd, &null))
		return (ft_free_prompt(shell, YES));;
	return (SUCCESS);
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
		if (ft_init_redirect_vecs(shell, &redir_count) == FAILURE) // TODO: FREE vars->input_line if malloc fails
			return (FAILURE);
		if (ft_parse_command_line(shell) == FAILURE)
			return (FAILURE);
		if (!vec_push(shell->pipes, &input))
			return (ft_free_prompt(shell, YES));
		ft_zero_redirects(&redir_count);
	}
	return (SUCCESS);
}
