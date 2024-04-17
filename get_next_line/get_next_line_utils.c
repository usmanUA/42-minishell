/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:57:57 by uahmed            #+#    #+#             */
/*   Updated: 2024/03/11 13:44:59 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	ft_freestr(char *s)
{
	if (s != NULL)
	{
		free(s);
		s = NULL;
	}
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

void	ft_copylines(char **new_line, char *line, char *lines)
{
	int	ind;
	int	i;

	ind = 0;
	if (line)
	{
		ind = -1;
		while (line[++ind])
			(*new_line)[ind] = line[ind];
	}
	i = 0;
	while (lines[i] && lines[i] != '\n')
	{
		(*new_line)[ind++] = lines[i];
		i++;
	}
	if (ft_newline(lines))
		(*new_line)[ind++] = '\n';
	(*new_line)[ind] = '\0';
}

char	*ft_writeline(char *lines, char *line)
{
	char	*new_line;
	size_t	lines_len;

	lines_len = ft_strlen(lines);
	if (ft_newline(lines))
		lines_len = ft_newline(lines);
	new_line = malloc(lines_len + ft_strlen(line) + 1);
	if (!new_line)
	{
		ft_freestr(line);
		return (NULL);
	}
	ft_copylines(&new_line, line, lines);
	ft_freestr(line);
	return (new_line);
}

size_t	ft_newline(char *s)
{
	size_t	ind;

	if (!s)
		return (0);
	ind = -1;
	while (s[++ind])
	{
		if (s[ind] == '\n')
			return (++ind);
	}
	return (0);
}
