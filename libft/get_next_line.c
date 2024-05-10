/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 13:09:53 by mkorpela          #+#    #+#             */
/*   Updated: 2024/02/09 15:09:02 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*join(char *lead_string, char *trailing_string, int bytes_read)
{
	int		i;
	int		j;
	char	*join_str;

	i = 0;
	j = 0;
	join_str = malloc(sizeof(char) * (f_strlen(lead_string) + bytes_read + 1));
	if (join_str == NULL)
		return (free_string_and_return_null(lead_string));
	while (i < f_strlen(lead_string))
	{
		join_str[i] = lead_string[i];
		i++;
	}
	while (i < f_strlen(lead_string) + bytes_read)
	{
		join_str[i] = trailing_string[j];
		j++;
		i++;
	}
	join_str[i] = '\0';
	free(lead_string);
	return (join_str);
}

static char	*next_line_start(char *string)
{
	char	*next_line;
	int		len_next_line;
	int		new_line_index;

	new_line_index = 0;
	len_next_line = 0;
	if (return_newline_index(string) != -1)
	{
		new_line_index = return_newline_index(string);
		new_line_index++;
		len_next_line = f_strlen(string) - new_line_index;
	}
	else
		len_next_line = 0;
	next_line = malloc(sizeof(char) * (len_next_line + 1));
	if (!next_line)
		return (free_string_and_return_null(string));
	string_copy(next_line, string + new_line_index, len_next_line);
	free(string);
	return (next_line);
}

static char	*make_line(char *string, char *line)
{
	int	line_len;

	line_len = return_newline_index(string);
	line_len++;
	if (line_len == 0)
		line_len = f_strlen(string);
	line = malloc(sizeof(char) * (line_len + 1));
	if (!line)
		return (free_string_and_return_null(string));
	string_copy(line, string, line_len);
	return (line);
}

static char	*make_string(int fd, char *string)
{
	char	buffer[BUFFER_SIZE];
	int		bytes_read;

	bytes_read = 1;
	string = make_empty_string(string);
	if (string == NULL)
		return (NULL);
	while (return_newline_index(string) == -1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free_string_and_return_null(string));
		if (bytes_read == 0)
		{
			if (!string[0])
				return (free_string_and_return_null(string));
			return (string);
		}
		string = join(string, buffer, bytes_read);
		if (!string)
			return (NULL);
	}
	return (string);
}

char	*get_next_line(int fd)
{
	char static	*string;
	char		*line;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE >= SIZE_MAX)
		return (NULL);
	string = make_string(fd, string);
	if (!string)
		return (NULL);
	line = make_line(string, line);
	if (!line)
	{
		string = NULL;
		return (NULL);
	}
	string = next_line_start(string);
	if (string == NULL)
		return (free_string_and_return_null(line));
	return (line);
}
