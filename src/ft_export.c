/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:29:57 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 13:30:03 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	ft_export(t_command *command, t_data *data)
{
	t_vars	*tmp;
	char	*to_export;
	int		i;

	i = 1;
	while (command->args[i])
	{
		if (is_valid_identifier(command->args[i]) == ERROR)
		{
			write(STDERR_FILENO, " not a valid identifier\n", 24);
			g_error = 1;
			i++;
			continue ;
		}
		to_export = mini_getenv(command->args[i], data->envp);
		if (to_export && mini_getvars(data->exp_vars, command->args[i]) == NULL)
		{
			set_variable(&data->exp_vars, command->args[i], to_export);
			sort_list(&data->exp_vars, data->exp_vars);
		}
		else if (!mini_getvars(data->vars, command->args[i])
				&& !mini_getvars(data->exp_vars, command->args[i]))
		{
			set_variable(&data->exp_vars, command->args[i], "");
			if (ft_strchr(command->args[i], '='))
				if (set_exp(data, command->args[i], "") == ERROR)
					return ; //limpiar y ver y tal
			sort_list(&data->exp_vars, data->exp_vars);
		}
		else if (mini_getvars(data->vars, command->args[i]))
		{
			to_export = mini_getvars(data->vars, command->args[i]);
			if (set_exp(data, command->args[i], to_export) == ERROR)
				return ; //limpiar y ver y tal
			set_variable(&data->exp_vars, command->args[i], to_export);
			unset_from_vars(command, &data->vars);
			sort_list(&data->exp_vars, data->exp_vars);
		}
		i++;
		g_error = 0;
	}
	if (i == 1)
	{
		tmp = data->exp_vars;
		while (tmp != NULL)
		{
			write(STDOUT_FILENO, "declare -x ", 11);
			write(STDOUT_FILENO, tmp->name, ft_strlen(tmp->name));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, tmp->value, ft_strlen(tmp->value));
			write(STDOUT_FILENO, "\n", 1);
			tmp = tmp->next;
		}
		g_error = 0;
	}
}
