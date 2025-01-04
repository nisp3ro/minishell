/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hexa_n_pointers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:19:22 by mrubal-c          #+#    #+#             */
/*   Updated: 2024/10/01 10:13:22 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_hexalen(unsigned long long num)
{
	int	len;

	len = 0;
	while (num != 0)
	{
		len++;
		num = num / 16;
	}
	return (len);
}

void	ft_hexa_interpreter(unsigned long long num, const char word)
{
	if (num >= 16)
	{
		ft_hexa_interpreter(num / 16, word);
		ft_hexa_interpreter(num % 16, word);
	}
	else
	{
		if (num < 10)
			ft_print_character(num + '0');
		else
		{
			if (word == 'X')
				ft_print_character(num - 10 + 'A');
			else
				ft_print_character(num - 10 + 'a');
		}
	}
}

int	ft_print_hexa(unsigned long long num, const char specifier)
{
	int	size;

	size = 0;
	if (specifier == 'p')
	{
		if (!num)
			return (ft_print_string("(nil)"));
		size += ft_print_string("0x");
	}
	if (num == 0)
		size += ft_print_character('0');
	else
	{
		ft_hexa_interpreter(num, specifier);
		size += ft_hexalen(num);
	}
	return (size);
}
