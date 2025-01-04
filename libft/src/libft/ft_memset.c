/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 13:06:40 by mrubal-c          #+#    #+#             */
/*   Updated: 2024/09/18 17:57:08 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t n)
{
	unsigned char	*pstr;
	unsigned char	value;
	size_t			i;

	i = 0;
	pstr = (unsigned char *)str;
	value = (unsigned char)c;
	while (i < n)
		pstr[i++] = value;
	return (str);
}
