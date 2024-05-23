/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_2d_envp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 12:24:06 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/23 12:45:43 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_index_of_envp_in_2d_array(char	**envp, char *key, int len_cmp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len_cmp) == 0)
		{
			break ;
		}
		i++;
	}
	return (i);
}

void	increment_2d_shell_level(t_shell *shell, char **envp)
{
	int		num_value;
	char	*value;
	char	*new_value;
	char	*new_envp;
	int		i;

	i = find_index_of_envp_in_2d_array(envp, "SHLVL=", 6);
	if (envp[i] == NULL)
		return ;
	value = ft_strchr(envp[i], '=');
	value++;
	num_value = ft_atoi(value);
	if (num_value < 0 || num_value >= 999)
		num_value = 0;
	else
		num_value++;
	new_value = ft_itoa(num_value);
	if (new_value == NULL)
		return ;
	new_envp = ft_strjoin("SHLVL=", new_value);
	free(new_value);
	if (new_envp == NULL)
		return ;
	free(envp[i]);
	shell->envp[i] = new_envp;
}

t_envp	*ft_listlast(t_envp *lst)
{
	while (lst != NULL)
	{
		if (lst->next == NULL)
			break ;
		lst = lst->next;
	}
	return (lst);
}

static int	ft_array_length(char **envp)
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
		exit (EXIT_FAILURE);
	}
	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (new_envp[i] == NULL)
		{
			free_failed_2d_array(shell, envp, i);
			exit (EXIT_FAILURE);
		}
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}
