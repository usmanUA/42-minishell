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
#include "miniwell.h"

static	char	*ft_find_value(t_vars *vars, t_envp *env_vars, char *key, int len)
{
	char	*value;
	t_envp	*envp;

	value = NULL;
	envp = env_vars;
	while (envp->next)
	{
		if (!ft_strncmp(key, envp->env_name, len))
		{
			vars->expanded = YES;
			value = ft_strdup(envp->env_value);
			break ;
		}
		envp = envp->next;
	}
	return (value);
}

static	char	*ft_join_str_expansion(t_vars *vars, t_envp *env_vars)
{
	char	*str;
	int		len;
	char	*key;
	char	*value;

	str = NULL;
	len = vars->ind - 1;
	while (++len < vars->end)
	{
		if (vars->input_line[len] == '$')
			break ;
	}
	str = ft_substr(vars->input_line, vars->ind, len);
	if (!str)
		return (NULL);
	++len;
	key = &vars->input_line[len];
	value = ft_find_value(vars, env_vars, key, vars->end - len);
	if (value)
		str = ft_strjoin(str, value);
	return (str);
}

static	char	*ft_expand_key(t_vars *vars, t_envp *env_vars, char *key, int op)
{
	char	*str;

	str = NULL;
	str = ft_find_value(vars, env_vars, key, vars->end-(vars->ind+1));
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

char	*ft_expand_variable(t_vars *vars, t_envp *env_vars, int op)
{
	char	*str;
	char	*key;

	if (vars->input_line[vars->ind] == '$')
	{
		key = &vars->input_line[vars->ind];
		if (!ft_strncmp(key, "$?", 2))
		{
			vars->expanded = YES;
			str = ft_itoa(vars->exit_status);
		}
		else
			str = ft_expand_key(vars, env_vars, key, op);
	}
	else
		str = ft_join_str_expansion(vars, env_vars);
	return (str);
}

