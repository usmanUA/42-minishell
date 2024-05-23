/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 10:26:39 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/22 10:26:41 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_init_strs(char **redir, char **fd, char **file)
{
	*redir = NULL;
	*fd = NULL;
	*file = NULL;
}

static void	ft_update_index(int redirect, int *ind, int count)
{
	int	no_use;

	no_use = redirect;
	*ind += count;
}

int	ft_redirection(t_vars *vars)
{
	int	ind;

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
