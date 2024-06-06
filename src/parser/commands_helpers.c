/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:49:11 by mkorpela          #+#    #+#             */
/*   Updated: 2024/06/05 14:49:12 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_push_env_var(t_shell *shell, char **s)
{
	int		ind;
	char	**env_var;

	ind = -1;
	env_var = NULL;
	env_var = ft_split(*s, ' ');
	free(*s);
	if (env_var == NULL)
		return (FAILURE);
	while (env_var[++ind])
	{
		if (!vec_push((*shell->input)->cmd, &env_var[ind]))
		{
			if (env_var)
				free(*env_var);
			return (ft_free_prompt(shell, YES));
		}
	}
	free(env_var);
	return (SUCCESS);
}
