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

static	char	*ft_find_value(t_vars *vars, t_envp *env_vars, char *key)
{
	char	*value;
	t_envp	*envp;
	int	len;

	len = ft_strlen(key);
	value = NULL;
	envp = env_vars;
	while (envp->next)
	{
		if (!ft_strncmp(key, envp->key, len))
		{
			vars->expanded = YES;
			value = ft_strdup(envp->value);
			break ;
		}
		envp = envp->next;
	}
	return (value);
}

static	char	*ft_expand_key(t_vars *vars, t_envp *env_vars, char *key, int op)
{
	char	*str;

	str = NULL;
	str = ft_find_value(vars, env_vars, key);
	if (op == FILENAME && vars->expand_it == YES && vars->expanded == NO)
	{
		key = ft_substr(vars->input_line, vars->ind, vars->end - vars->ind);
		if (!key)
			return (NULL);
		ft_filerror(0, key, YES);
		free(key);
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

char	*ft_expand_variable(t_vars *vars, t_envp *env_vars, int op)
{
	char	*str;
	char	*key;
	int	end;

	end = vars->ind + 1;
	if (vars->input_line[end] == '?')
	{
		str = ft_itoa(vars->exit_status);
		++end;
	}
	else
	{
		key = ft_substr(vars->input_line, vars->ind+1, vars->end - (vars->ind + 1));
		str = ft_expand_key(vars, env_vars, key, op);
		free(key);
	}
	// if (end < vars->end - vars->ind)
	// 	vars->qontinue = YES;
	return (str);
}

