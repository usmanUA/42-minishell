/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:36:22 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/24 15:36:24 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_free_redirect_strs_prompt(char **fd, char **redir, char **file,
			t_shell *shell);

static	int	ft_close_fd_free_redirect_strs(t_shell *shell, int fd)
{
	close(fd);
	return (ft_free_redirect_strs_prompt(shell->vars->f_des,
			shell->vars->redir, shell->vars->file, shell));
}

static	void	write_line(char **line, int fd)
{
	write(fd, *line, ft_strlen(*line));
	free(*line);
	*line = NULL;
	ft_putstr_fd(G, STDOUT_FILENO);
	ft_putstr_fd("> ", STDOUT_FILENO);
	ft_putstr_fd(T, STDOUT_FILENO);
	*line = get_next_line(0);
}

int	ft_get_line(t_shell *shell, char *eof, int fd)
{
	char	*line;

	line = NULL;
	ft_putstr_fd(G, STDOUT_FILENO);
	ft_putstr_fd("> ", STDOUT_FILENO);
	ft_putstr_fd(T, STDOUT_FILENO);
	line = get_next_line(STDIN_FILENO);
	while (42)
	{
		if (line == NULL)
			return (ft_close_fd_free_redirect_strs(shell, fd));
		if (eof == NULL)
		{
			if (line[0] == '\n')
				break ;
		}
		else if (((ft_strlen(line) - 1) == ft_strlen(eof) && !ft_strncmp(line, eof,
					ft_strlen(eof))) || !line)
			break ;
		write_line(&line, fd);
	}
	free(line);
	close(fd);
	return (SUCCESS);
}
