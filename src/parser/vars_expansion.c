/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:31:10 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/16 11:31:12 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_skip_special_expansions(t_shell *shell)
{
	char	next;

	next = shell->vars->input_line[shell->vars->ind + 1];
	if (ft_isdigit(next) || next == '-')
	{
		shell->vars->ind += 2;
		shell->vars->end += 2;
	}
	else if (next == '\"' || next == '\'')
	{
		++shell->vars->ind;
		++shell->vars->end;
	}
	else
		return ;
	ft_index_after_spaces(shell->vars);
}

static char	*ft_find_value(t_shell *shell, char **key)
{
	char	*value;
	t_envp	*envp;

	value = NULL;
	envp = shell->env_list;
	while (envp)
	{
		if (!ft_strcmp(*key, envp->key) && envp->value != NULL)
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

static char	*ft_expand_key(t_shell *shell, char **key, int op)
{
	char	*str;
	char	*temp;

	str = NULL;
	temp = &(*key)[1];
	str = ft_find_value(shell, &temp);
	if (op == FILENAME && shell->vars->expand_it == YES
		&& shell->vars->expanded == NO
		&& shell->vars->qontinue == NO)
		ft_filerror(0, *key, YES);
	free(*key);
	key = NULL;
	return (str);
}

char	*ft_expand_variable(t_shell *shell, int op)
{
	char	*str;
	char	*key;
	int		end;

	end = shell->vars->ind + 1;
	if (shell->vars->input_line[end] == '?')
	{
		str = ft_itoa(shell->vars->exit_status);
		shell->vars->expanded = YES;
	}
	else
	{
		key = ft_substr(shell->vars->input_line, shell->vars->ind,
				shell->vars->end - shell->vars->ind);
		if (key == NULL)
			return (NULL);
		str = ft_expand_key(shell, &key, op);
	}
	return (str);
}
