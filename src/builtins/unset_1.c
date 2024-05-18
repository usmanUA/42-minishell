/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:25:01 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/17 14:15:56 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniwell.h"

t_envp	*go_to_node_above_unset_node(t_shell *data, t_envp *unset_node)
{
	t_envp	*temp;

	temp = data->env_list;
	while (temp->next != unset_node)
	{
		temp = temp->next;
	}
	return (temp);
}

void	copy_and_set_new_array(t_shell *data, char *command, char **envp)
{
	int		len_cmp;
	int		i;
	int		j;

	len_cmp = ft_strlen(command);
	i = 0;
	j = 0;
	while (data->envp[j] != NULL)
	{
		// printf("%d: %s\n", j, data->envp[j]);
		if (ft_strncmp(command, data->envp[j], len_cmp) == 0 && data->envp[j][len_cmp] == '=')
		{
			// printf("-----------------------FREE-------------------------\n");
			free(data->envp[j]);
			j++;
		}
		else
		{
			envp[i] = data->envp[j];
			// envp[i] = ft_strdup(data->envp[j]);	
			i++;
			j++;
		}
	}
	envp[i] = NULL;
	free(data->envp);
	// free_env_array(data);
	data->envp = envp;
}

int	delete_env_from_array(t_shell *data, char *command)
{
	char	**envp;
	int		num_of_envs;

	num_of_envs = ft_array_length(data->envp);
	num_of_envs--;
	envp = malloc(sizeof(char *) * (num_of_envs + 1));
	if (envp == NULL)
	{
		return (1);
	}
	copy_and_set_new_array(data, command, envp);
	return (0);
}

bool	check_unset_syntax(char *argument)
{
	int	i;

	i = 0;
	if (ft_isalpha(argument[0]) == 0 && argument[0] != '_')
	{
		error_msg_hardcode("unset", argument, 5, true);
		return (true);
	}
	i++;
	while (argument[i])
	{
		if (ft_isalnum(argument[i]) == 0 && argument[i] != '_')
		{
			error_msg_hardcode("unset", argument, 5, true);
			return (true);
		}
		i++;
	}
	return (false);
}

int	unset_command(t_shell *data, char **command)
{
	int		i;
	bool	arg_error;
	bool	error_flag;

	if (command[1] == NULL)
		return (0);
	i = 1;
	error_flag = false;
	while (command[i])
	{
		arg_error = false;
		arg_error = check_unset_syntax(command[i]);
		if (arg_error == true)
			error_flag = true;
		if (arg_error == false)
		{
			if (delete_env_from_array(data, command[i]) == 1)
				error_flag = true;
			delete_environmental_variable(data, command[i]);
		}
		i++;
	}
	return (ft_return_value(error_flag));
}
