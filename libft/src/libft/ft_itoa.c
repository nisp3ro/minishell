/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 15:29:11 by mrubal-c          #+#    #+#             */
/*   Updated: 2024/09/20 16:29:35 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_recur_itoa(unsigned int n, char *res, int i)
{
	if (n > 9)
	{
		ft_recur_itoa((n / 10), res, (i - 1));
		res[i] = ((n % 10) + 48);
	}
	else
		res[i] = n + 48;
}

static int	ft_numlen(unsigned int n)
{
	int	len;

	len = 0;
	while (n > 9)
	{
		n /= 10;
		len++;
	}
	len++;
	return (len);
}

char	*ft_itoa(int n)
{
	int				sign;
	int				len;
	unsigned int	nb;
	char			*res;

	sign = 0;
	len = 0;
	if (n < 0)
	{
		sign = 1;
		nb = -n;
	}
	else
		nb = n;
	len = ft_numlen(nb);
	res = (char *)malloc(sizeof(char) * (sign + len + 1));
	if (!res)
		return (NULL);
	if (sign == 1)
		res[0] = '-';
	ft_recur_itoa(nb, res, (sign + len - 1));
	res[sign + len] = '\0';
	return (res);
}
