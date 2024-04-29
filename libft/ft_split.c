/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:26:55 by uahmed            #+#    #+#             */
/*   Updated: 2023/10/31 20:09:01 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static size_t	ft_totwords(char const *s, char k)
{
	size_t	words;

	words = 0;
	while (*s)
	{
		while (*s == k)
			s++;
		if (*s)
			words++;
		while (*s != k && *s)
			s++;
	}
	return (words);
}

static size_t	ft_wordlen(char const *s, char k)
{
	size_t	wl;
	char	*str;

	str = ft_strchr(s, k);
	if (!str)
		wl = ft_strlen(s);
	else
		wl = str - s;
	return (wl);
}

static char	*ft_strsub(char const *s, unsigned int st, size_t len)
{
	char	*sbs;
	size_t	ind;

	sbs = (char *)malloc(len + 1);
	if (!sbs)
		return (NULL);
	ind = -1;
	while (++ind < len)
		sbs[ind] = s[st + ind];
	sbs[ind] = 0;
	return (sbs);
}

char	**ft_split(char const *s, char c)
{
	t_var	vars;

	if (s == NULL)
		return (NULL);
	vars.rstr = (char **)malloc(sizeof(char *) * (ft_totwords(s, c) + 1));
	if (!vars.rstr)
		return (0);
	vars.ind = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s)
		{
			vars.word_l = ft_wordlen(s, c);
			vars.rstr[vars.ind] = ft_strsub(s, 0, vars.word_l);
			if (!vars.rstr[vars.ind])
			{
				ft_free(vars.rstr, vars.ind);
				return (NULL);
			}
			s += vars.word_l;
			vars.ind++;
		}
	}
	vars.rstr[vars.ind] = 0;
	return (vars.rstr);
}
