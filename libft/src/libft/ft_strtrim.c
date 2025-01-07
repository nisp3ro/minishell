/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 21:34:23 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/05 14:27:41 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		i;
	int		istart;
	int		iend;

	if (!s1 || !set)
		return (NULL);
	istart = 0;
	iend = ft_strlen(s1);
	while (ft_strchr(set, s1[istart]) && s1[istart])
		istart++;
	while (iend > istart && ft_strchr(set, s1[iend - 1]))
		iend--;
	if (iend <= istart)
		return (ft_calloc(1, 1));
	res = (char *)malloc(sizeof(char) * (iend - istart + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (istart < iend)
		res[i++] = s1[istart++];
	res[i] = '\0';
	return (res);
}
