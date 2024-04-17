/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 18:16:46 by uahmed            #+#    #+#             */
/*   Updated: 2024/03/04 14:56:49 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "../libft/libft.h"

# ifndef FDS
#  define FDS 256
# endif

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

size_t	ft_strlen(const char *s);
char	*ft_writeline(char *lines, char *line);
size_t	ft_newline(char *s);
void	ft_freebuff(char *lines);
int		ft_movebuff(char *lines);
int		ft_errorcheck(int fd, char *lines);
char	*get_next_line(int fd);
#endif
