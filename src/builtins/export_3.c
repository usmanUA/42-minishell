/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 07:41:39 by mkorpela          #+#    #+#             */
/*   Updated: 2024/06/05 14:38:18 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_returnvalue(char *return_variable)
{
	if (return_variable == NULL)
		return (1);
	else
		return (0);
}

static bool	append_to_value(char *command)
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

static char	*append_value(t_shell *shell, char *command, t_envp *node)
{
	char	*value_append;
	char	*new_value;	

	(void)shell;
	value_append = get_value_of_env_var(shell, command);
	if (value_append == NULL)
	{
		return (NULL);
	}
	new_value = ft_strjoin(node->value, value_append);
	free(value_append);
	free(node->value);
	if (new_value == NULL)
	{
		shell->malloc_status = FAILURE;
		return (NULL);
	}
	return (new_value);
}

int	if_env_var_exists(t_shell *shell, char *command)
{
	t_envp	*node;

	node = search_for_envp(shell, command);
	if (node != NULL)
	{
		if (ft_strchr(command, '=') != NULL)
		{
			if (append_to_value(command) == false)
			{
				free(node->value);
				node->value = get_value_of_env_var(shell, command);
				return (ft_returnvalue(node->value));
			}
			else if (node->value != NULL)
			{
				node->value = append_value(shell, command, node);
				return (ft_returnvalue(node->value));
			}
		}
	}
	return (0);
}
