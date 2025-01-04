/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 20:13:40 by mrubal-c          #+#    #+#             */
/*   Updated: 2024/09/18 20:42:27 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*res;
	int		is;
	int		ir;

	is = 0;
	ir = 0;
	res = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (res == NULL)
		return (NULL);
	while (s1[is])
	{
		res[ir] = s1[is];
		ir++;
		is++;
	}
	is = 0;
	while (s2[is])
	{
		res[ir] = s2[is];
		ir++;
		is++;
	}
	res[ir] = '\0';
	return (res);
}
