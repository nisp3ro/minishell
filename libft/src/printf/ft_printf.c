/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:19:04 by mrubal-c          #+#    #+#             */
/*   Updated: 2024/10/01 10:13:14 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_select_format(va_list arg, const char specifier)
{
	int	size;

	size = 0;
	if (specifier == 'c')
		size += ft_print_character(va_arg(arg, int));
	else if (specifier == 's')
		size += ft_print_string(va_arg(arg, char *));
	else if (specifier == 'd' || specifier == 'i')
		size += ft_print_number(va_arg(arg, int));
	else if (specifier == 'u')
		size += ft_print_unsigned(va_arg(arg, unsigned int));
	else if (specifier == 'x' || specifier == 'X')
		size += ft_print_hexa(va_arg(arg, unsigned int), specifier);
	else if (specifier == 'p')
		size += ft_print_hexa(va_arg(arg, unsigned long long), specifier);
	else
		size += ft_print_character(specifier);
	return (size);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	int		size;
	va_list	arg;

	i = 0;
	size = 0;
	va_start(arg, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			size += ft_select_format(arg, str[i + 1]);
			i++;
		}
		else
			size += ft_print_character(str[i]);
		i++;
	}
	va_end(arg);
	return (size);
}
