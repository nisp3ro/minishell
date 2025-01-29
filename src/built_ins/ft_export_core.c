/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:29:57 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/28 16:47:55 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	unset_from_vars_export(char *command, t_vars **vars)
{
	t_vars	*tmp;
	t_vars	*prev;

	tmp = *vars;
	prev = NULL;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, command, ft_strlen(command)) == 0
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

static void	export_new_variable(t_command *command, t_data *data, int i)
{
	char	*to_export;

	to_export = mini_getenv(command->args[i], data->envp);
	if (to_export && mini_getvars(data->exp_vars, command->args[i]) == NULL)
	{
		set_variable(&data->exp_vars, command->args[i], to_export);
		sort_list(&data->exp_vars, data->exp_vars);
	}
}

static void	export_empty_variable(t_command *command, t_data *data, int i)
{
	special_set_exp(data, command->args[i], "");
	set_variable(&data->exp_vars, command->args[i], "");
	sort_list(&data->exp_vars, data->exp_vars);
}

static void	export_existing_variable(t_command *command, t_data *data, int i)
{
	char	*to_export;

	to_export = mini_getvars(data->vars, command->args[i]);
	if (set_exp(data, command->args[i], to_export) == ERROR)
		return ;
	set_variable(&data->exp_vars, command->args[i], to_export);
	unset_from_vars_export(command->args[i], &data->vars);
	sort_list(&data->exp_vars, data->exp_vars);
}

void	ft_export(t_command *command, t_data *data)
{
	int	i;

	i = 1;
	g_exit_code = 0;
	while (command->args[i])
	{
		if (is_valid_identifier(command->args[i]) == ERROR)
		{
			handle_invalid_identifier(command, &i);
			continue ;
		}
		if (!mini_getvars(data->vars, command->args[i])
			&& !mini_getvars(data->exp_vars, command->args[i]))
			export_empty_variable(command, data, i);
		else if (mini_getvars(data->vars, command->args[i]))
			export_existing_variable(command, data, i);
		else
			export_new_variable(command, data, i);
		i++;
	}
	if (i == 1)
		print_exported_vars(data);
}
