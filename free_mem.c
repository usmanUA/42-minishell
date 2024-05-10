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

static void	ft_free_redirect(t_vec **redirect)
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

static void	ft_free_input(void **inpt)
{
	t_input	*input;

	input = *(t_input **)inpt;
	if (input)
	{
		ft_free_cmd(input->cmd);
		if (input->new_fds)
			ft_free_redirect(&input->new_fds);
		if (input->orig_fds)
			ft_free_redirect(&input->orig_fds);
		if (input->fds_info)
			ft_free_redirect(&input->fds_info);
		free(input->file_flag);
		free(input);
	}
}
void	ft_free_vec(t_shell *shell)
{
	int	ind;

	ind = -1;
	vec_free(shell->info);
	free(shell->info);
	ind = -1;
	while (++ind < (int)shell->pipes->len)
		ft_free_input(vec_get(shell->pipes, ind));
	vec_free(shell->pipes);
	free(shell->pipes);
	if (shell->vars->unlink_here_doc == YES)
		unlink(".here_doc");
	free(shell->vars);
}
