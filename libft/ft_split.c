/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:26:55 by uahmed            #+#    #+#             */
/*   Updated: 2024/03/08 17:07:57 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stdio.h"

static size_t	ft_totwords(char const *s, char *set, char k, int flag)
{
	size_t	words;
	char	temp;

	words = 0;
	while (*s)
	{
		while (*s == k)
			s++;
		if (*s)
			words++;
		if (flag && ft_ispresent(set, *s))
		{
			temp = *s;
			s = ft_strchr(++s, temp);
			s++;
		}
		else
		{
			while (*s != k && *s)
				s++;
		}
	}
	return (words);
}

static size_t	ft_wordlen(char const *s, char k, int flag)
{
	size_t	wl;
	char	*str;
	int		spaces;

	spaces = 0;
	if (flag)
	{
		while (*s == k)
		{
			spaces++;
			s++;
		}
	}
	str = ft_strchr(s, k);
	if (!str)
		wl = ft_strlen(s);
	else
		wl = str - s;
	return (wl + spaces);
}

static char	*ft_implement_split(t_var *vars, const char **s, char *set, char c)
{
	char	*substr;

	substr = NULL;
	if (!vars->flag)
	{
		while (**s == c && **s)
			(*s)++;
	}
	if (**s)
	{
		if (set && ft_ispresent(set, **s))
		{
			c = **s;
			(*s)++;
		}
		vars->word_l = ft_wordlen(*s, c, vars->flag);
		substr = ft_substr(*s, 0, vars->word_l);
		(*s) += vars->word_l;
		if (vars->flag && **s)
			(*s)++;
	}
	return (substr);
}

char	**ft_split(char const *s, char *set, char c, int flag)
{
	t_var	vars;

	vars.flag = flag;
	if (!s)
		return (NULL);
	vars.str = (char **)malloc(sizeof(char *) * (ft_totwords(s,
					set, c, flag) + 1));
	if (!vars.str)
		return (0);
	vars.ind = 0;
	vars.word_l = 0;
	while (*s)
	{
		vars.str[vars.ind] = ft_implement_split(&vars, &s, set, c);
		if (!vars.str[vars.ind])
		{
			ft_free(vars.str, vars.ind + 1);
			return (0);
		}
		vars.ind++;
		if (set && ft_ispresent(set, *s) && *(s + 1) == '\0')
			s++;
	}
	vars.str[vars.ind] = 0;
	return (vars.str);
}
