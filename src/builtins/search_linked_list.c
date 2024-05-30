/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_linked_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:12:16 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/29 16:01:33 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*string_n_copy(char *dest, char *source, int len)
{
	int	i;

	if (dest == NULL || source == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dest[i] = source[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static int	get_name_length(char *envp_string)
{
	int	i;

	if (envp_string == NULL)
	{
		return (0);
	}
	i = 0;
	while (envp_string[i] && envp_string[i] != '=')
	{
		i++;
	}
	if (envp_string[i - 1] == '+')
	{
		i--;
	}
	return (i);
}

char	*get_name_of_env_variable(char *envp_string)
{
	char	*name;
	int		name_length;

	name_length = get_name_length(envp_string);
	name = (char *)malloc(sizeof(char) * (name_length + 1));
	if (name == NULL)
	{
		return (NULL);
	}
	name = string_n_copy(name, envp_string, name_length);
	return (name);
}

char	*get_value_of_env_variable(char *envp_string)
{
	char	*value;
	int		value_length;
	char	*value_string;

	value = ft_strchr(envp_string, '=');
	if (value == NULL)
		return (NULL);
	value++;
	value_length = f_strlen(value);
	value_string = (char *)malloc(sizeof(char) * (value_length + 1));
	if (value_string == NULL)
	{
		return (NULL);
	}
	value_string = string_n_copy(value_string, value, value_length);
	return (value_string);
}

t_envp	*search_for_envp(t_shell *shell, char *command)
{
	int		name_length;
	t_envp	*temp;

	name_length = get_name_length(command);
	temp = shell->env_list;
	while (temp && temp->key)
	{
		if (ft_strncmp(command, temp->key, name_length) == 0)
		{
			if (temp->key[name_length] == '\0' || temp->key[name_length] == '+')
			{
				return (temp);
			}
		}
		temp = temp->next;
	}
	return (NULL);
}
