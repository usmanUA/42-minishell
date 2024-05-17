/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 15:05:37 by mkorpela          #+#    #+#             */
/*   Updated: 2023/11/20 11:08:03 by mkorpela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	substring_count(char const *s1, char c)
{
	int	i;
	int	subs_cnt;

	i = 0;
	subs_cnt = 0;
	while (s1[i] != '\0')
	{
		while (s1[i] == c && s1[i] != '\0')
		{
			i++;
		}
		if (s1[i])
		{
			subs_cnt++;
		}
		while (s1[i] != c && s1[i] != '\0')
		{
			i++;
		}
	}
	return (subs_cnt);
}

static int	len_of_substr(char const *s1, size_t *i, char c, size_t *start_i)
{
	size_t	j;

	j = 0;
	while (s1[*i] == c && s1[*i] != '\0')
	{
		(*i)++;
	}
	*start_i = *i;
	while (s1[*i] != c && s1[*i] != '\0')
	{
		(*i)++;
		j++;
	}
	return (j);
}

static char	*malloc_arr_of_str(char **arr_of_str, size_t k, size_t len)
{
	int	i;

	i = (int)k;
	arr_of_str[i] = (char *)malloc((len + 1) * (sizeof(char)));
	if (arr_of_str[i] == NULL)
	{
		i--;
		while (i >= 0)
		{
			free(arr_of_str[i]);
			i--;
		}
		free(arr_of_str);
		return (NULL);
	}
	return (arr_of_str[i]);
}

static char	**all_ft(size_t subs_cnt, char **arr_of_str, char const *s1, char c)
{
	size_t	i;
	size_t	j;
	size_t	k;
	size_t	start_i;
	size_t	len;

	i = 0;
	k = 0;
	while (k < subs_cnt)
	{
		len = len_of_substr(s1, &i, c, &start_i);
		if (!malloc_arr_of_str(arr_of_str, k, len))
			return (NULL);
		j = 0;
		while (j < len)
		{
			arr_of_str[k][j] = s1[start_i];
			j++;
			start_i++;
		}
		arr_of_str[k][j] = '\0';
		k++;
	}
	arr_of_str[k] = NULL;
	return (arr_of_str);
}

char	**ft_split(char const *s1, char c)
{
	size_t	subs_cnt;
	char	**arr_of_str;

	subs_cnt = substring_count(s1, c);
	arr_of_str = malloc((subs_cnt + 1) * sizeof(char *));
	if (arr_of_str == NULL)
	{
		return (NULL);
	}
	return (all_ft(subs_cnt, arr_of_str, s1, c));
}
