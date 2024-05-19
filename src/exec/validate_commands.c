/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:15:13 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/14 10:08:04 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_splitted_paths(char **paths);
char	*ft_join_path(char *path, char *command);
char	*ft_give_path(char **envp);
int	ft_check_dir(t_pipex *pipex);
void	ft_check_builtin(t_shell *shell, char *command, t_pipex *pipex);

static int	ft_check_command(char *command, t_pipex *pipex, int check_dir)
{
	if (!access(command, F_OK))
	{
		if (check_dir == YES)
		{
			if (ft_check_dir(pipex) == INVALID)
				return (INVALID);
		}
		if (access(command, X_OK) == -1)
		{
			ft_cmd_error(pipex->command, 0, 1);
			pipex->cmd_flag = YELLOW;
			return (INVALID);
		}
	}
	else
	{
		pipex->cmd_flag = RED;
		return (INVALID);
	}
	return (VALID);
}

static	void	ft_handle_absolute(t_shell *shell, t_pipex *pipex)
{
	int	fd;

	fd = -2;
	ft_check_command(pipex->command, pipex, YES);
	// NOTE: if absolute command (given) does not exist
	if (pipex->cmd_flag == RED)
		ft_cmd_error(pipex->command, 0, 0); // TODO: define MACROS for const. values
}

static int	ft_make_command(t_pipex *pipex, char **paths)
{
	char	*cmd_path;
	int		i;

	i = -1;
	while (paths[++i])
	{
		cmd_path = ft_join_path(paths[i], pipex->command);
		if (!cmd_path)
			return (FAILURE);
		if (ft_check_command(cmd_path, pipex, NO) == VALID)
		{
			vec_insert((*pipex->input)->cmd, &cmd_path, 0);
			vec_remove((*pipex->input)->cmd, 1);           
			free(pipex->command);               
			pipex->cmd_flag = GREEN;
			break ;
		}
		else
			free(cmd_path);
	}
	return (SUCCESS);
}

void	ft_handle_relative(t_pipex *pipex, t_shell *shell, char **paths)
{
	pipex->cmd_flag = RED;
	if (ft_make_command(pipex, paths) == FAILURE)
	{
		shell->vars->malloc_flag = RED;
		ft_free_prompt(shell, NO);
		return;
	}
	// NOTE: if it gets here it means command does not exits
	// TODO: return exit status 127
	if (pipex->cmd_flag == RED)
		ft_cmd_error(pipex->command, 1, 1);
}

void	ft_validate_commands(t_pipex *pipex, t_shell *shell)
{
	char	**paths;

	pipex->command = *(char **)vec_get((*pipex->input)->cmd, 0);
	if (pipex->command == NULL)
	{
		shell->vars->malloc_flag = RED;
		return;
	}
	paths = ft_split(ft_give_path(shell->envp), ':');
	if (!paths)
	{
		shell->vars->malloc_flag = RED;
		ft_free_prompt(shell, NO);
		return;
	}
	if (ft_ispresent(pipex->command, '/'))
		ft_handle_absolute(shell, pipex);
	else
	{
		ft_check_builtin(shell, pipex->command, pipex);
		if (pipex->exec_type == EXTERNAL) 
			ft_handle_relative(pipex, shell, paths);
	}
	ft_free_splitted_paths(paths);
}
