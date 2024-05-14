/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:31:10 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/07 16:31:13 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static	char	*ft_find_value(t_shell *shell, char **key)
{
	char	*value;
	t_envp	*envp;
	int	len;

	len = ft_strlen(*key);
	value = NULL;
	envp = shell->env_list;
	while (envp->next)
	{
		if (!ft_strncmp(*key, envp->key, len))
		{
			shell->vars->expanded = YES;
			value = ft_strdup(envp->value);
			if (value == NULL)
			{
				free(*key);
				ft_free_prompt(shell, YES);
			}
			break ;
		}
		envp = envp->next;
	}
	return (value);
}

static	char	*ft_expand_key(t_shell *shell, char **key, int op)
{
	char	*str;

	str = NULL;
	str = ft_find_value(shell, key);
	free(*key);
	key = NULL;
	if (op == FILENAME && shell->vars->expand_it == YES && shell->vars->expanded == NO)
	{
		*key = ft_substr(shell->vars->input_line, shell->vars->ind, shell->vars->end - shell->vars->ind);
		if (!(*key))
		{
			if (str)
				free(str);
			return (NULL); // NOTE: free str before exit?
		}
		ft_filerror(0, *key, YES);
		free(*key);
	}
	return (str);
}

char	*ft_parse_key(t_vars *vars, int *end)
{
	char	*key;
	char	c;
	int	start;

	start = *end;
	c = vars->input_line[*end];
	while (c == '_' || ft_isalnum(c))
	{
		++(*end);
		c = vars->input_line[*end];
	}
	key = ft_substr(vars->input_line, start, *end);
	return (key);	
}

char	*ft_expand_variable(t_shell *shell, int op)
{
	char	*str;
	char	*key;
	int	end;

	end = shell->vars->ind + 1;
	if (shell->vars->input_line[end] == '?')
	{
		str = ft_itoa(shell->vars->exit_status); // NOTE: can itoa return NULL in cases other than malloc fail?
		++end;
	}
	else
	{
		key = ft_substr(shell->vars->input_line, shell->vars->ind+1, shell->vars->end - (shell->vars->ind + 1));
		if (key == NULL)
			return (NULL);
		str = ft_expand_key(shell, &key, op);
	}
	// if (end < vars->end - vars->ind)
	// 	vars->qontinue = YES;
	return (str);
}

