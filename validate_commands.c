#include "miniwell.h"
#include "vec/vec.h"

char	*ft_join_path(char *path, t_info *info, char *command)
{
	char	*path_to_cmd;
	char	*cmd_path;

	path_to_cmd = ft_strjoin(path, "/");
	if (!path_to_cmd)
		return (NULL);
	cmd_path = ft_strjoin(path_to_cmd, command);
	if (!cmd_path)
	{
		free(path_to_cmd);
		return (NULL);
	}
	if (path_to_cmd)
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
		return (path);
	return (&path[5]);
}

void	ft_handle_absolute(char *command, t_info *info)
{
	int	fd;

	fd = -2;
	if (!access(command, F_OK))
	{
		fd = open(command, O_DIRECTORY);
		if (fd != -1)
		{
			close(fd);
			ft_cmd_error(command, 2, 1);
			// TODO: return relevant exit status as it is a directory 
		}
		else if (access(command, X_OK) == -1)
		{
			ft_cmd_error(command, 0, 1);
			// TODO: command no permission, 
			// return 126 exit status
		}
		else
			fd = 0; // NOTE: bogus entry
			// TODO: command is OK, continue executing
	}
	else
	{
		// NOTE: absolute command (given) does not exist
		ft_cmd_error(command, 0, 0); // TODO: define MACROS for const. values
		// TODO: return exit status 127
	}
}

void	ft_make_command(t_info *info)
{
}

void	ft_handle_relative(t_vec *cmd, t_info *info, char **paths)
{
	char	*cmd_path;
	char	*command;
	int		i;

	i = -1;
	command = *(char **)vec_get(cmd, 0);
	while (paths[++i])
	{
		cmd_path = ft_join_path(paths[i], info, command);
		if (!access(cmd_path, F_OK))
		{
			if (access(cmd_path, X_OK) == -1)
			{
				ft_cmd_error(command, 0, 1);
				info->cmd_permission = 1;
				// TODO: return exit status 126	
			}
			else
			{
				// TODO: replace command with absolute path command in cmd vec
			}	
			return ;
		}
		free(cmd_path);
	}
	// NOTE: if it comes here it means command does not exits
	// // NOTE: if it comes here it means command does not exits
	// TODO: return exit status 127
	info->no_cmd = 1;
	ft_cmd_error(command, 1, 1);
}

int	ft_validate_commands(t_input *input, t_info *info, char **envp)
{
	char **paths;
	char *command;
	
	if (*input->file_flag == RED)
		return (1);
	paths = ft_split(ft_give_path(envp), ':');
	if (!paths)
		return (0);
	command = *(char **)vec_get(input->cmd, 0);
	if (ft_ispresent(command, '/'))
		ft_handle_absolute(command, info);
	else
		ft_handle_relative(input->cmd, info, paths);
	return (1);
}

