/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_begins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:55:03 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/24 13:50:23 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_redirection(t_vars *vars);
void		ft_count_redirs(t_vars *vars, t_redir_count *redir_count);
void		ft_zero_redirects(t_redir_count *redir_count);
void		ft_skip_special_expansions(t_shell *shell);

void	ft_index_after_spaces(t_vars *vars)
{
	char	c;

	c = vars->input_line[vars->end];
	while (c != '\0' && ft_isspace(c))
	{
		vars->end++;
		c = vars->input_line[vars->end];
	}
	vars->ind = vars->end;
}

void	ft_skip_pipe(t_shell *shell)
{
	if (shell->vars->input_line[shell->vars->ind] == '|')
	{
		shell->vars->end++;
		ft_index_after_spaces(shell->vars);
		if (shell->vars->input_line[shell->vars->ind] == '\0')
			ft_token_error('|', 0);
	}
}

static int	ft_parsing_action(t_shell *shell)
{
	char	c;

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

	null = NULL;
	input = shell->input;
	cmd = (t_vec *)malloc(sizeof(t_vec));
	if (!cmd)
		return (ft_free_prompt(shell, YES));
	(*input)->cmd = cmd;
	vec_new(cmd, 0, sizeof(char **));
	file_flag = (int *)malloc(sizeof(int));
	if (!file_flag)
		return (ft_free_prompt(shell, YES));
	*file_flag = GREEN;
	(*input)->file_flag = file_flag;
	if (ft_parsing_action(shell) == FAILURE)
		return (FAILURE);
	if (!vec_push(cmd, &null))
		return (ft_free_prompt(shell, YES));
	return (SUCCESS);
}

int	ft_save_input(t_shell *shell)
{
	t_input			*input;
	t_redir_count	redir_count;

	ft_zero_redirects(&redir_count);
	ft_index_after_spaces(shell->vars);
	while (shell->vars->input_line[shell->vars->ind] != '\0')
	{
		ft_count_redirs(shell->vars, &redir_count);
		shell->vars->redir_count = &redir_count;
		input = malloc(sizeof(t_input));
		if (!input)
			return (ft_free_prompt(shell, NO));
		shell->input = &input;
		if (ft_init_redirect_vecs(shell, &redir_count) == FAILURE)
			return (FAILURE);
		if (ft_parse_command_line(shell) == FAILURE)
			return (FAILURE);
		if (!vec_push(shell->pipes, &input))
			return (ft_free_prompt(shell, YES));
		ft_zero_redirects(&redir_count);
	}
	return (SUCCESS);
}
