/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 10:09:00 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/25 10:09:02 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	free_env_list(t_shell *data)
{
	t_envp	*prev_node;
	t_envp	*curr_node;
	
	curr_node = data->env_list;
	while (curr_node)
	{
		prev_node = curr_node;
		curr_node = curr_node->next;
		free(prev_node->key);
		free(prev_node->value);
		free(prev_node);
	}
	data->env_list = NULL;
}

static	void	ft_free_redirect(t_vec **redirect)
{
	int	ind;
	int	fd;

	ind = -1;
	if (redirect && *redirect)
	{
		while (++ind < (int)(*redirect)->len)
		{
			fd = (*redirect)->mem[ind * (*redirect)->size];
			if (fd > 2)
				close(fd);
		}
		vec_free(*redirect);
		free(*redirect);
	}
}

static void	ft_free_cmd(t_vec *cmd)
{
	char	*str;
	int		ind;

	ind = -1;
	if (cmd)
	{
		while (++ind < (int)cmd->len)
		{
			str = *(char **)vec_get(cmd, ind);
			if (str)
				free(str);
		}
		vec_free(cmd);
		free(cmd);
	}
}

void	ft_free_input(void **inpt)
{
	t_input	*input;

	input = *(t_input **)inpt;
	if (input)
	{
		ft_free_cmd(input->cmd);
		ft_free_redirect(&input->new_fds);
		ft_free_redirect(&input->orig_fds);
		ft_free_redirect(&input->fds_info);
		free(input->file_flag);
		free(input);
	}
}

void	ft_free_shell(t_shell *shell, int free_env)
{
	int	ind;

	if (free_env == YES)
		free_env_list(shell);
	if (shell->info)
	{
		vec_free(shell->info);
		free(shell->info);
	}
	ind = -1;
	if (shell->pipes)
	{
		while (++ind < (int)shell->pipes->len)
			ft_free_input(vec_get(shell->pipes, ind));
		vec_free(shell->pipes);
		free(shell->pipes);
	}
	if (shell->pids)
	{
		vec_free(shell->pids);
		free(shell->pids);
	}
	if (shell->vars->unlink_here_doc == YES)
		unlink(".here_doc");
	free(shell->vars);
}
