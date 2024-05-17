 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_linkedlist.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:12:16 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/09 10:13:08 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_name_length(char *envp_string)
{
	int	i;

	if (envp_string == NULL)//This might not be necessary. But adds segfault protection.
	{
		return (0);
	}
	i = 0;
	while (envp_string[i] && envp_string[i] != '=')
	{
		i++;
	}
	if (envp_string[i] == '+')
	{
		i--;
	}
	// ft_putendl_fd(envp_string, 2);
	// ft_putnbr_fd(i, 2);
	// printf("envp_string: %s\n", envp_string);
	// printf("name_length: %d\n", i);	
	return (i);
}

char	*get_name_of_env_variable(t_shell *data, char *envp_string)
{
	char	*name;
	int		name_length;

	name_length = get_name_length(envp_string);
	name = (char *)malloc(sizeof(char) * (name_length + 1));
	if (name == NULL)
	{
		free_env_list(data);
		exit (1);
	}
	name = string_n_copy(name, envp_string, name_length);
	return (name);
}

char	*get_value_of_env_variable(t_shell *data, char *envp_string)
{
	char	*value;
	int		value_length;
	char	*value_string;

	value = ft_strchr(envp_string, '=');//what if no equals
	//if null error
	value++;
	value_length = f_strlen(value);
	value_string = (char *)malloc(sizeof(char) * (value_length + 1));
	if (value_string == NULL)
	{
		free_env_list(data);
		exit (1);
	}
	value_string = string_n_copy(value_string, value, value_length);
	return (value_string);
}

t_envp	*search_for_envp(t_shell *data, char *command)
{
	int		name_length;
	t_envp	*temp;

	name_length = get_name_length(command);

	temp = data->env_list;
	while (temp && temp->key)
	{
		if (ft_strncmp(command, temp->key, name_length) == 0)
		{
			if (temp->key[name_length] == '\0' || temp->key[name_length] == '+')
			{
				// printf("envp exists: %s\n", command);
				return (temp);
			}
		}
		temp = temp->next;
	}
	// printf("envp does not exist\n");
	return (NULL);
}
