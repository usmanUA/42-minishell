
#include "minishell.h"


int		ft_free_redirect_strs_prompt(char **fd, char **redir, char **file,
			t_shell *shell);

int	ft_get_line(t_shell *shell, char *eof, int fd)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	while (42)
	{
		if (line == NULL)
		{
			close(fd);
			return (ft_free_redirect_strs_prompt(shell->vars->f_des,
				shell->vars->redir, shell->vars->file, shell));
		}
		if (((ft_strlen(line) - 1) == ft_strlen(eof) && !strncmp(line, eof,
					strlen(eof))) || !line)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
		line = NULL;
		line = get_next_line(0);
	}
	close(fd);
	return (SUCCESS);
}
