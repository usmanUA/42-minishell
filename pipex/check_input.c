/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 12:05:21 by uahmed            #+#    #+#             */
/*   Updated: 2024/03/15 16:33:20 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_validate_files(char *infile, char *outfile, t_pipex *ppx)
{
	ppx->fd_in = open(infile, O_RDONLY);
	if (ppx->fd_in == -1)
		ft_filerror(errno, ppx, 1, 1);
	if (ppx->here_doc)
		ppx->fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0666);
	else
		ppx->fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (ppx->fd_out == -1)
	{
		ppx->fdout_errno = errno;
		ft_filerror(errno, ppx, 0, 0);
	}
}

static void	ft_malloc_struct(t_pipex *pipex, char ***cmds, char ****cmd_args,
		int idx)
{
	if (idx)
	{
		pipex->cmd_args[pipex->idx] = (char **)malloc(2 * sizeof(char *));
		if (!pipex->cmd_args[pipex->idx])
			ft_exit_error(pipex, 1, EXIT_FAILURE, 0);
		return ;
	}
	*cmds = (char **)malloc((pipex->tot_cmds + 1) * sizeof(char *));
	if (!(*cmds))
		ft_exit_error(pipex, 1, EXIT_FAILURE, 0);
	*cmd_args = (char ***)malloc((pipex->tot_cmds + 1) * sizeof(char **));
	if (!(*cmd_args))
		ft_exit_error(pipex, 1, EXIT_FAILURE, 0);
}

static int	ft_null_case(t_pipex *ppx)
{
	ppx->cmds[ppx->idx] = NULL;
	ppx->cmd_args[ppx->idx] = NULL;
	if (ft_notwrite_cmderror(ppx))
		return (1);
	ft_cmd_error("", 1, 1);
	return (1);
}

static int	ft_nullstr_spaces(t_pipex *ppx, char **args)
{
	int	i;

	i = -1;
	if (args[ppx->idx + ppx->start][0] == '\0')
		return (ft_null_case(ppx));
	else
	{
		while (args[ppx->idx + ppx->start][++i])
		{
			if (!ft_isspace(args[ppx->idx + ppx->start][i]))
				return (0);
		}
		ppx->cmds[ppx->idx] = NULL;
		ft_malloc_struct(ppx, NULL, NULL, 1);
		ppx->cmd_args[ppx->idx][0] = ft_strdup(args[ppx->idx + ppx->start]);
		ppx->cmd_args[ppx->idx][1] = NULL;
		if (ft_notwrite_cmderror(ppx))
			return (1);
		ft_cmd_error(args[ppx->idx + ppx->start], 1, 1);
		return (1);
	}
	return (0);
}

void	ft_validate_commands(char **args, char **envp, t_pipex *ppx)
{
	ft_malloc_struct(ppx, &ppx->cmds, &ppx->cmd_args, 0);
	ppx->paths = ft_split(ft_give_path(envp), NULL, ':', 0);
	while (++ppx->idx < ppx->tot_cmds)
	{
		if (ft_nullstr_spaces(ppx, args))
			continue ;
		ppx->cmd_args[ppx->idx] = ft_split(args[ppx->idx + ppx->start], "\'\"",
				' ', 1);
		if (ft_ispresent(ppx->cmd_args[ppx->idx][0], '/'))
			ft_handle_absolute(ppx);
		else
			ft_handle_relative(ppx);
	}
	ppx->cmds[ppx->idx] = NULL;
	ppx->cmd_args[ppx->idx] = NULL;
}
