/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:26:58 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 20:06:30 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_pwd(t_data *data)
{
	if (data->pwd == NULL)
	{
		write(STDERR_FILENO, "pwd: cannot get the current directory", 37);
		g_exit_code = 1;
		return ;
	}
	write(STDOUT_FILENO, data->pwd, ft_strlen(data->pwd));
	write(STDOUT_FILENO, "\n", 1);
	g_exit_code = 0;
}
