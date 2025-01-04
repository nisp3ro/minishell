/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 15:45:31 by mrubal-c          #+#    #+#             */
/*   Updated: 2024/09/18 17:57:02 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t n)
{
	size_t	i1;
	size_t	i2;

	i1 = 0;
	if (s2[0] == '\0')
		return ((char *)s1);
	while (i1 < n && s1[i1])
	{
		i2 = 0;
		while ((i1 + i2) < n && s1[i1 + i2] == s2[i2])
		{
			i2++;
			if (s2[i2] == '\0')
				return ((char *)&s1[i1]);
		}
		i1++;
		if ((i1 + i2) >= n)
			return (0);
	}
	return (0);
}
