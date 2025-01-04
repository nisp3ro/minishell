/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_numbers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:19:32 by mrubal-c          #+#    #+#             */
/*   Updated: 2024/10/01 10:13:18 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_print_unsigned(unsigned int n)
{
	int	size;

	size = 0;
	if (n == 0)
		size += ft_print_character('0');
	else
	{
		if (n > 9)
			size += ft_print_unsigned(n / 10);
		size += ft_print_character((n % 10) + '0');
	}
	return (size);
}

int	ft_print_number(int n)
{
	int				size;
	unsigned int	nb;

	size = 0;
	if (n == 0)
		size += ft_print_character('0');
	else if (n < 0)
	{
		size += ft_print_character('-');
		nb = -n;
		size += ft_print_unsigned(nb);
	}
	else if (n > 0)
	{
		nb = n;
		size += ft_print_unsigned(nb);
	}
	return (size);
}
