/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 14:55:44 by mrubal-c          #+#    #+#             */
/*   Updated: 2024/09/20 20:47:12 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_freeall(char **res, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(res[i]);
		i++;
	}
	free(res);
}

static int	ft_wordcounter(char const *str, char c)
{
	int	isword;
	int	words;

	isword = 0;
	words = 0;
	while (*str)
	{
		while (*str == c)
		{
			isword = 0;
			str++;
		}
		if (*str != c && *str != '\0' && isword == 0)
			words += 1;
		while (*str != c && *str)
		{
			isword = 1;
			str++;
		}
	}
	return (words);
}

static int	ft_splitter(char **res, char const *str, char c, int words)
{
	int	wcount;
	int	len;
	int	i;

	wcount = -1;
	while (++wcount < words)
	{
		i = 0;
		len = 0;
		while (*str == c)
			str++;
		while (str[i] != c && str[i++])
			len++;
		res[wcount] = (char *)malloc(sizeof(char) * (len + 1));
		if (!res[wcount])
		{
			ft_freeall(res, wcount);
			return (0);
		}
		i = 0;
		while (i < len)
			res[wcount][i++] = *str++;
		res[wcount][i] = '\0';
	}
	return (1);
}

char	**ft_split(char const *str, char c)
{
	int		words;
	char	**res;

	if (!str)
		return (NULL);
	words = ft_wordcounter(str, c);
	res = (char **)malloc(sizeof(char *) * (words + 1));
	if (!res)
		return (NULL);
	if (!(ft_splitter(res, str, c, words)))
		return (NULL);
	res[words] = NULL;
	return (res);
}
