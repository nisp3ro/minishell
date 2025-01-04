/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 19:01:09 by mrubal-c          #+#    #+#             */
/*   Updated: 2024/09/18 17:57:18 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	void	*spc;

	spc = malloc(num * size);
	if (spc == NULL)
		return (NULL);
	ft_memset(spc, 0, (num * size));
	return (spc);
}
