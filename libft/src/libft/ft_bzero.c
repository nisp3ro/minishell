/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 13:38:59 by mrubal-c          #+#    #+#             */
/*   Updated: 2024/09/18 17:57:19 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *str, size_t n)
{
	unsigned char	*pstr;
	size_t			i;

	i = 0;
	pstr = (unsigned char *)str;
	while (i < n)
	{
		pstr[i] = 0;
		i++;
	}
}
