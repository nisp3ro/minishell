/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:26:58 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/30 13:05:49 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_pwd(t_data *data)
{
	if (data->pwd == NULL)
	{
		write(STDERR_FILENO, "pwd: cannot get the current directory", 37);
		data->g_exit_code = 1;
		return ;
	}
	write(STDOUT_FILENO, data->pwd, ft_strlen(data->pwd));
	write(STDOUT_FILENO, "\n", 1);
	data->g_exit_code = 0;
}
