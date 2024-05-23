/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:28:19 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/22 16:07:47 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_envp_2d_array(char **envp)//Del this FUNCTION before eval - this is only for confirming
{
	int	i;

	i = 0;
	while (envp[i])
	{
		ft_printf("%d: %s\n", i, envp[i]);
		i++;
	}
}

char	*create_empty_string(void)
{
	char	*string;

	string = (char *)malloc(sizeof(char) * 1);
	if (string == NULL)
	{
		return (NULL);
	}
	string[0] = '\0';
	return (string);
}

int	ft_return_value(int error_flag)
{
	if (error_flag == true)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] - s2[i] != 0)
		{
			return (s1[i] - s2[i]);
		}
		i++;
	}
	return (0);
}
