/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_char_n_strings.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:19:15 by mrubal-c          #+#    #+#             */
/*   Updated: 2024/10/01 10:13:25 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_print_character(int character)
{
	write(1, &character, 1);
	return (1);
}

int	ft_print_string(char *str)
{
	int	size;

	size = 0;
	if (str == 0)
	{
		return (ft_print_string("(null)"));
	}
	while (str[size])
	{
		size += ft_print_character(str[size]);
	}
	return (size);
}
