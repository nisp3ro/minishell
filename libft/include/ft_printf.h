/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:16:29 by mrubal-c          #+#    #+#             */
/*   Updated: 2024/10/01 10:13:30 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

int		ft_printf(const char *str, ...);
int		ft_select_format(va_list arg, const char specifier);
int		ft_print_unsigned(unsigned int n);
int		ft_print_number(int n);
int		ft_print_hexa(unsigned long long num, const char specifier);
void	ft_hexa_interpreter(unsigned long long num, const char word);
int		ft_hexalen(unsigned long long num);
int		ft_print_string(char *str);
int		ft_print_character(int character);

#endif
