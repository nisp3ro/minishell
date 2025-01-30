/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 16:44:45 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/30 13:05:31 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	i = 0;
	if (ft_isdigit(str[0]) || str[0] == '=')
		return (ERROR);
	while (str[i] != '\0' && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (ERROR);
		i++;
	}
	return (OK);
}

void	print_exported_vars(t_data *data)
{
	t_vars	*tmp;

	tmp = data->exp_vars;
	while (tmp != NULL)
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		write(STDOUT_FILENO, tmp->name, ft_strlen(tmp->name));
		write(STDOUT_FILENO, "=\"", 2);
		write(STDOUT_FILENO, tmp->value, ft_strlen(tmp->value));
		write(STDOUT_FILENO, "\"\n", 2);
		tmp = tmp->next;
	}
}

void	handle_invalid_identifier(t_data *data, int *i)
{
	write(STDERR_FILENO, " not a valid identifier\n", 24);
	data->g_exit_code = 1;
	(*i)++;
}
