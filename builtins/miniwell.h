/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniwell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 11:50:48 by mkorpela          #+#    #+#             */
/*   Updated: 2024/05/09 14:35:36 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIWELL_H
#define MINIWELL_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <sys/errno.h>
#include "../libft/libft.h"
#include "../minishell.h"

int		ft_strcmp(char *s1, char *s2);
// void	print_env_list(t_shell *data);
char	*string_n_copy(char *dest, char *source, int len);



#endif
