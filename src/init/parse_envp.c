
#include "minishell.h"
#include <stdio.h>

static	void	free_failed_2d_array(t_shell *shell, char **envp, int i)
{
	i--;
	while (i >= 0)
	{
		free(envp[i]);
		i--;
	}
	free(envp);
	shell->envp = NULL;
}

void	free_env_array(t_shell *data)
{
	int	i;
	
	if (data->envp == NULL)
	{
		return;
	}
	i = 0;
	while (data->envp[i])//does this work?
	{
		free(data->envp[i]);
		i++;
	}
	free(data->envp);
	data->envp = NULL;
}

int	ft_array_length(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		i++;
	}
	return (i);
}

char	**malloc_envp(t_shell *shell, char **envp)
{
	char	**new_envp;
	int		i;
	int		num_of_envs;

	num_of_envs = ft_array_length(envp);
	new_envp = malloc(sizeof(char *) * (num_of_envs + 1));
	if (!new_envp)
	{
		ft_free_prompt(shell, YES);
		exit (EXIT_FAILURE);
	}
	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (new_envp[i] == NULL)
		{
			free_failed_2d_array(shell, envp, i);
			ft_free_prompt(shell, YES);
			exit (EXIT_FAILURE);
		}
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}
