/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:25:09 by mrubal-c          #+#    #+#             */
/*   Updated: 2024/09/18 17:57:11 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *str1, const void *str2, size_t n)
{
	const unsigned char	*s1;
	const unsigned char	*s2;
	size_t				i;

	i = 0;
	s1 = (const unsigned char *)str1;
	s2 = (const unsigned char *)str2;
	if (n == 0)
		return (0);
	while (s1[i] == s2[i])
	{
		if ((i + 1) == n)
			return ((int)(s1[i] - s2[i]));
		i++;
	}
	return ((int)(s1[i] - s2[i]));
}
