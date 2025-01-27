/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:29:57 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/27 12:40:50 by mrubal-c         ###   ########.fr       */
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

static void	unset_from_vars_export(char *command, t_vars **vars)
{
	t_vars	*tmp;
	t_vars	*prev;

	tmp = *vars;
	prev = NULL;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, command,
				ft_strlen(command)) == 0
			&& tmp->name[ft_strlen(command)] == '\0')
		{
			if (prev)
				prev->next = tmp->next;
			else
				*vars = tmp->next;
			free(tmp->name);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	ft_export(t_command *command, t_data *data)
{
	t_vars	*tmp;
	char	*to_export;
	int		i;

	i = 1;
	g_exit_code = 0;
	while (command->args[i])
	{
		if (is_valid_identifier(command->args[i]) == ERROR)
		{
			write(STDERR_FILENO, " not a valid identifier\n", 24);
			g_exit_code = 1;
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
			sort_list(&data->exp_vars, data->exp_vars);
		}
		else if (mini_getvars(data->vars, command->args[i]))
		{
			to_export = mini_getvars(data->vars, command->args[i]);
			if (set_exp(data, command->args[i], to_export) == ERROR)
				return ; //limpiar y ver y tal
			set_variable(&data->exp_vars, command->args[i], to_export);
			unset_from_vars_export(command->args[i], &data->vars);
			sort_list(&data->exp_vars, data->exp_vars);
		}
		i++;
	}
	if (i == 1)
	{
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
}
