
#include "minishell.h"

void	ft_free_splitted_paths(char **paths)
{
	int	ind;

	ind = -1;
	while (paths[++ind])
		free(paths[ind]);
	free(paths);
}

char	*ft_join_path(char *path, char *command)
{
	char	*path_to_cmd;
	char	*cmd_path;

	path_to_cmd = NULL;
	path_to_cmd = ft_strjoin(path, "/");
	if (!path_to_cmd)
		return (NULL);
	cmd_path = ft_strjoin(path_to_cmd, command);
	if (!cmd_path)
	{
		free(path_to_cmd);
		return (NULL);
	}
	free(path_to_cmd);
	return (cmd_path);
}

char	*ft_give_path(char **envp)
{
	char	*path;

	path = NULL;
	while (*envp)
	{
		if (!ft_strncmp(*envp, "PATH=", 5))
		{
			path = *envp;
			break ;
		}
		envp++;
	}
	if (!path)
		return (NULL);
	return (&path[5]);
}

int	ft_check_dir(t_pipex *pipex)
{
	int	fd;

	fd = -2;
	fd = open(pipex->command, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		ft_cmd_error(pipex->command, 2, 1); // NOTE: is a directory
		pipex->cmd_flag = YELLOW;
		// TODO: return relevant exit status 126
		close(fd);
		return (INVALID);
	}
	close(fd);
	return (VALID);
}

void	ft_check_builtin(t_shell *shell, char *command, t_pipex *pipex)
{
	int	executable_info;

	executable_info = EXTERNAL;
	if (ft_strcmp(command, "cd") == 0)
		executable_info = CD;
	if (ft_strcmp(command, "echo") == 0)
		executable_info = MY_ECHO;
	if (ft_strcmp(command, "env") == 0)
		executable_info = ENV;
	if (ft_strcmp(command, "exit") == 0)
		executable_info = EXIT;
	if (ft_strcmp(command, "export") == 0)
		executable_info = EXPORT;
	if (ft_strcmp(command, "pwd") == 0)
		executable_info = PWD;
	if (ft_strcmp(command, "unset") == 0)
		executable_info = UNSET;
	pipex->exec_type = executable_info;
}

