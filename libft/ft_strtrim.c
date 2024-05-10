/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:52:20 by mkorpela          #+#    #+#             */
/*   Updated: 2023/11/19 21:48:00 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*copy_trim_str(size_t i, size_t j, char	*str_trim, char const *s1)
{
	size_t	k;

	k = 0;
	while (i <= j)
	{
		str_trim[k] = s1[i];
		k++;
		i++;
	}
	str_trim[k] = '\0';
	return (str_trim);
}

static size_t	calc_right(char const *s1, char const *set, size_t *j)
{
	size_t	m;

	m = 0;
	*j = ft_strlen(s1) - 1;
	while (s1[*j] >= 0)
	{
		m = 0;
		while (s1[*j] != set[m] && m < ft_strlen(set))
			m++;
		if (s1[*j] == set[m])
			(*j)--;
		else if ((*j) > 4294967295)
		{
			*j = ft_strlen(s1);
			break ;
		}
		else
			break ;
	}
	return (*j);
}

static size_t	l_and_r(char const *s1, char const *set, size_t *i, size_t *j)
{
	size_t	m;

	while (s1[*i] != '\0')
	{
		m = 0;
		while (s1[*i] != set[m] && m < ft_strlen(set))
			m++;
		if (s1[*i] == set[m])
			(*i)++;
		else
			break ;
	}
	*j = calc_right(s1, set, j);
	return ((*j) - (*i) + 1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	size_t	str_trim_len;
	char	*str_trim;

	i = 0;
	j = 0;
	if (ft_strlen(s1) == 0)
	{
		str_trim = malloc(1);
		if (str_trim == NULL)
			return (NULL);
		str_trim[0] = '\0';
		return (str_trim);
	}
	if (ft_strlen(set) == 0)
		return (ft_strdup(s1));
	else
		str_trim_len = l_and_r(s1, set, &i, &j);
	str_trim = (char *)malloc((str_trim_len + 1) * sizeof(char));
	if (str_trim == NULL)
		return (NULL);
	str_trim = copy_trim_str(i, j, str_trim, s1);
	return (str_trim);
}
