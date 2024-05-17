/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:39:59 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/16 11:15:44 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_export_syntax(char *argument)
{
	int	i;

	i = 0;
	if (ft_isalpha(argument[0]) == 0 && argument[0] != '_')
	{
		error_msg_hardcode("export", argument, 5, true);
		// printf("bash: export: `%s': not a valid identifier\n", argument);
		// printf("Error in First Character - it should be either an Uppercase, Lowercase Letter or Underscore\n");
		return (true);
	}
	i++;
	while (argument[i] && argument[i] != '=')
	{
		if (ft_isalnum(argument[i]) == 0 && argument[i] != '_')
		{
			if ((argument[i]) == '+' && argument[i+1] == '=')
			{
				break;
			}
			error_msg_hardcode("export", argument, 5, true);
			// printf("bash: export: `%s': not a valid identifier\n", argument);
			// printf("Error after first character. Should be alphabet (upper or lower), digit or underscore.\n");
			return (true);
		}
		i++;
	}
	return (false);
}

bool	append_to_value(t_shell *data, char *command)
{
	bool	append;
	char	*value;
	
	append = false;
	value = ft_strchr(command, '=');
	value--;
	if (*value == '+')
	{
		append = true;
	}
	return (append);
}

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

void	append_to_2d_envp_array(t_shell *data, char *command)
{
	t_envp	*node;

	node = search_for_envp(data, command);
	
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

// void	update_env_2d_array(t_shell *data, char *command)
// {
// 	t_envp	*node;
// 	int		key_len;
// 	int		key_len2;
// 	int		i;

// 	node = search_for_envp(data, command);
// 	key_len = ft_strlen(node->key);
// 	key_len2 = get_key_len(data, command);
// 	if (key_len != key_len2)
// 	{
// 		printf("key_len(%d) != key_len2(%d)", key_len, key_len2);
// 		return ;
// 	}
// 	i = 0;
// 	while (data->envp)
// 	{
// 		if (ft_strncmp())
// 		i++;
// 	}

// }

void	if_env_var_exists(t_shell *data, char *command)
{
	t_envp	*node;
	char	*value_append;
	char	*new_value;

	node = search_for_envp(data, command);
	if (node != NULL)
	{
		// printf("env_variable already exists\n");
		//If there is a value after equals sign
		if (ft_strchr(command, '=') != NULL)
		{
			// node->value = NULL;
			if (append_to_value(data, command) == false)
			{
				free(node->value);
				node->value = get_value_of_env_variable(data, command);
				
/*				2D array change value			*/
				
				//ADD TO LIST
			}
			else			//APPEND
			{
				value_append = get_value_of_env_variable(data, command);
				new_value = ft_strjoin(node->value, value_append);
				free(value_append);
				free(node->value);
				node->value = new_value;
/*				append_to_2d_envp_array(data, command);		*/

			}
/*			update_env_2d_array(data, command);				*/
			// printf("env_variable VALUE changed\n");				
			// printf("%s=%s\n", node->key, node->value);
		}
		else
		{
			/*DO NOT ADD TO 2D array of envs*/
			
			// printf("env_variable Value NOT changed\n");
			// printf("%s=%s\n", node->key, node->value);
		}
	}
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

// char	*create_new_env_string(t_shell *data, char *command)
// {
// 	int		string_len;
// 	char	*new_env;
	
// 	string_len = get_new_string_length(command);
// 	new_env = malloc(sizeof(char) * (string_len + 1));
// 	{
// 		//free all
// 		exit (1);
// 	}
// 	new_env = string_copy_env(new_env, command);
// 	return (new_env);
// }







// void	add_env_to_envp_2d_array(t_shell *data, char *command)
// {
// 	char	*new_env;

// 	new_env = create_a_new_env_string(data, command);
	
	
// }

void	if_env_var_does_not_exist(t_shell *data, char *command)
{
	t_envp	*node;
	
	node = search_for_envp(data, command);//CHANGE THIS...-> DONE
	if (node == NULL)
	{
		node = create_new_node(data);
		node->key = get_name_of_env_variable(data, command);//CHANGE THIS...
		if (ft_strchr(command, '=') == NULL)
		{
			node->value = NULL;
			/*Do NOT add to envp 2D array*/

			// printf("New environmental variable created with NULL-value\n");
			// printf("%s=%s\n", node->key, node->value);				
		}
		else
		{
			node->value = get_value_of_env_variable(data, command);		
			// add_env_to_envp_2d_array(data, command);
			// printf("New environmental variable created with non-NULL-value\n");
			// printf("%s=%s\n", node->key, node->value);
		}
		ft_listadd_back(&(data->env_list), node);
		// if (ft_strchr(command, '=') != NULL)
		// {
		// 	add_to_envp_2d_array(data, command);
		// }
		// printf("%s=%s\n", node->key, node->value);
	}
}

void	print_in_almost_alphabetical_order(t_shell *data, char *j)
{
	t_envp	*temp;
	// char	i;
	
	// i = 'S';
	temp = data->env_list;
	while (temp)
	{
		if (temp->key[0] == *j)
		{
			printf("declare -x %s", temp->key);
			if (temp->value != NULL)
			{
				printf("=\"%s\"", temp->value);
			}
			printf("\n");
		}
		temp = temp->next;
	}
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

void	update_2d_envp_array(t_shell *data, char *command)
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
		append = true;
	}
	else
	{
		env_string = ft_strdup(command);
		if (env_string == NULL)
		{
			//free all
			exit (1);
		}
	}
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
		//add to end of list
	}
}

int	export_command(t_shell *data, char **command)
{
	int		i;
	char	j;
	bool	error;
	bool	error_flag;;
	
	// printf("Hello export function\n");
	if (command[1] == NULL)
	{
		// print_in_alphabetical_order(data);
		j = ' ';
		while (j < 127)
		{
			print_in_almost_alphabetical_order(data, &j);
			j++;
		}
		return (0);
	}
	if (command[1] != NULL)
	{
		i = 1;
		error = false;
		error_flag = false;
		while (command[i])
		{
			error = check_export_syntax(command[i]);
			if (error == true)
			{
				error_flag = true;
			}
			if (error == false)
			{
				if_env_var_exists(data, command[i]);
				if_env_var_does_not_exist(data, command[i]);
				update_2d_envp_array(data, command[i]);
			}
			i++;
		}
	}
	if (error_flag == true)
	{
		printf("1\n");
		return (1);
	}
	else
	{
		printf("0\n");
		return (0);
	}

}
