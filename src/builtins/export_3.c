/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 10:15:20 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/18 10:16:29 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniwell.h"





char	*create_a_new_env_string(t_shell *data, char *command)
{
	t_envp	*node;
	char	*key_with_equals;
	char	*new_env;

	node = search_for_envp(data, command);
	key_with_equals = ft_strjoin(node->key, "=");
	// printf("key_with_equals: %s\n", key_with_equals);
	if (key_with_equals == NULL)
	{
		//Free all
		exit (1);	
	}
	new_env = ft_strjoin(key_with_equals, node->value);
	// printf("new_env: %s\n", new_env);	
	free(key_with_equals);
	if (new_env == NULL)
	{
		//Free all
		exit (1);
	}
	return (new_env);
}

void	add_to_envp_2d_array(t_shell *data, char *command)
{
	char	**new_env_list;	
	int		num_of_envs;
	int		i;
	int		j;
	
	// printf("A\n");
	num_of_envs = ft_array_length(data->envp);
	// printf("num_of_envs ORIGINAL: %d\n", num_of_envs);
	
	// printf("B\n");
	num_of_envs++;
	// printf("num_of_envs: %d\n", num_of_envs);
	new_env_list = malloc(sizeof(char *) * (num_of_envs + 1));
	if (new_env_list == NULL)
	{
		//free all
		exit(1);
	}
	// printf("C\n");
	i = 0;
	j = 0;
	while (data->envp[j])
	{
		new_env_list[i] = data->envp[j];
		// printf("%d: %s\n", j, data->envp[j]);
		i++;
		j++;
	}
	// printf("D\n");
	// new_env_list[i] = ft_strdup(command);
	new_env_list[i] = create_a_new_env_string(data, command);
	// printf("%d: %s\n", i, new_env_list[i]);
	// printf("E\n");
	// new_env_list[i] = create_new_env_string(data, command);
	// if (new_env_list[i] == NULL)
	// {
	// 	//free all
	// 	exit (1);
	// }
	i++;
	new_env_list[i] = NULL;
	//free data->envp
	data->envp = new_env_list;
}

int	get_key_len(t_shell *data, char *command)
{
	int	i;

	i = 0;
	while (command[i] && command[i] != '+')
	{
		i++;
	}
	return (i);
}



int	get_new_string_length(char *command)
{
	int	i;
	int	string_len;
	
	i = 0;
	while (command[i])
	{
		if (command[i] == '+' && command[i+1] == '=')
		{
			i++;
		}
		else
		{
			i++;
			string_len++;
		}
	}
	return (string_len);
}

char	*string_copy_env(char *new_env, char *command)
{
	bool	first_equals_sign;
	int		i;
	int		j;
	
	first_equals_sign = false;
	i = 0;
	j = 0;
	while (command[i])
	{
		if (command[i] == '=')
		{
			first_equals_sign = true;
		}
		if (command[i] == '+' && command[i + 1] == '=' && first_equals_sign == false)
		{
			i++;
		}
		else
		{
			new_env[j] = command[i];
			i++;
			j++;
		}
	}
	new_env[j] = '\0';
	return (new_env);
}



int	len_until_char(t_shell *data, char *command, char c)
{
	int	i;

	i = 0;
	while (command[i] && command[i] != c)
	{
		i++;
	}
	return (i);
}

bool	key_exists_in_2d_array(t_shell *data, char *command)
{
	int	len_compare;
	int	i;

	i = 0;
	len_compare = len_until_char(data, command, '=');
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], command, len_compare) == 0)
		{
			if (data->envp[i][len_compare + 1] == '=')
			{
				return (true);
			}
		}
		i++;
	}
	return (false);
}

void	append_value(t_shell *data, char *command, int i)
{
	char	*new_env;
	char	*value_to_append;

	value_to_append = ft_strchr(command, '=');
	value_to_append++;
	new_env = ft_strjoin(data->envp[i], value_to_append);
	if (new_env == NULL)
	{
		//free all
		exit (1);
	}
	free(data->envp[i]);
	data->envp[i] = new_env;
}

void	append_value_to_env(t_shell *data, char *command)
{
	int		len_compare;
	int		i;

	i = 0;
	len_compare = len_until_char(data, command, '=');
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], command, len_compare) == 0)
		{
			if (data->envp[i][len_compare + 1] == '=')
			{
				append_value(data, command, i);
				return ;
			}
		}
		i++;
	}
}

void	change_envp_value(t_shell *data, char *command, int i)
{
	free(data->envp[i]);
	data->envp[i] = ft_strdup(command);
	if (data->envp[i] == NULL)
	{
		//free_all
		exit (1);
	}
}

void	change_envp_to_new_value(t_shell *data, char *command)
{
	int		len_compare;
	int		i;

	i = 0;
	len_compare = len_until_char(data, command, '=');
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], command, len_compare) == 0)
		{
			if (data->envp[i][len_compare + 1] == '=')
			{
				change_envp_value(data, command, i);
				return ;
			}
		}
		i++;
	}
}

bool	does_argument_append(t_shell *data, char *command)
{
	int	i;
	
	i = 0;
	while (command[i])
	{
		if (command[i] == '=')
		{
			return (false);	
		}
		if (command[i] == '+' && command[i + 1] == '=')
		{
			return (true);
		}
		i++;
	}
	return (false);
}

bool	there_is_key_but_no_value(t_shell *data, char *command)
{
	int i;

	i = 0;
	while (command[i])
	{
		if (command[i] == '=')
		{
			return (false);
		}
		i++;
	}
	return (true);
}

char	*string_copy_without_plus(char *new_env, char *command)
{
	bool	first_plus_sign;
	int		i;
	int		j;
	
	first_plus_sign = false;
	i = 0;
	j = 0;
	while (command[j])
	{
		if (command[j] == '+' && first_plus_sign == false)
		{
			first_plus_sign = true;
			j++;
		}
		else
		{
			new_env[i] = command[j];
			i++;
			j++;
		}
	}
	new_env[j] = '\0';
	return (new_env);
}

char	*make_string_without_plus_sign(t_shell *data, char *command)
{
	char	*new_env;
	int		string_len;

	string_len = ft_strlen(command);
	string_len--;
	new_env = malloc(sizeof(char *) * (string_len + 1));
	if (new_env == NULL)
	{
		//free all
		exit (1);
	}
	new_env = string_copy_without_plus(new_env, command);
	return (new_env);
}

void	update_2d_envp_array(t_shell *data, char *command)//Fix this function when you have it in you. 
{
	char	*env_string;
	bool	append;
	
	append = false;
	if (there_is_key_but_no_value(data, command) == true)
	{
		return ;
	}
	if (does_argument_append(data, command) == true)
	{
		//Make string
		env_string = make_string_without_plus_sign(data, command);
		if (env_string == NULL)
			return ;
		append = true;
	}
	else
	{
		env_string = ft_strdup(command);
		if (env_string == NULL)
		{
			return ;
		}
	}
	//free command and then set it to new string??
	if (key_exists_in_2d_array(data, command) == true)
	{
		if (append == true)
		{
			//strjoin
			append_value_to_env(data, command);
		}
		else
		{
			change_envp_to_new_value(data, command);
		}
	}
	else
	{
		add_to_envp_2d_array(data, command);
	}
}
