/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_assigment.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:39:00 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/26 15:39:36 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*create_env_entry(char *name, char *value)
{
	size_t	size;
	char	*entry;

	size = ft_strlen(name) + ft_strlen(value) + 2;
	entry = malloc(size);
	if (!entry)
		return (NULL);
	ft_strcpy(entry, name);
	ft_strcat(entry, "=");
	ft_strcat(entry, value);
	return (entry);
}

void	replace_user_variable(char *existing_var, char *value, t_data *data,
		char *name)
{
	free(existing_var);
	existing_var = ft_strdup(value);
	if (!existing_var)
		return (perror("malloc"));
	if (!mini_getvars(data->exp_vars, name))
	{
		set_exp(data, name, value);
	}
}

bool	process_environment_variable(char *name, char *value, t_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], name, ft_strlen(name)) == 0 &&
			data->envp[i][ft_strlen(name)] == '=')
		{
			free(data->envp[i]);
			data->envp[i] = create_env_entry(name, value);
			if (!data->envp[i])
			{
				perror("malloc");
				exit(1);
			}
			return (true);
		}
		i++;
	}
	return (false);
}

void	process_user_variable(char *name, char *value, t_vars **env_vars,
		t_data *data)
{
	char	*existing_var;

	existing_var = mini_getvars(data->exp_vars, name);
	if (existing_var)
	{
		replace_user_variable(existing_var, value, data, name);
		set_exp(data, name, value);
	}
	else
	{
		existing_var = mini_getvars(data->vars, name);
		if (existing_var)
			replace_user_variable(existing_var, value, data, name);
		else
			set_variable(env_vars, name, value);
	}
}

bool	handle_variable_assignment(char *input, t_vars **env_vars, t_data *data)
{
	char	*expanded;
	char	*equal_sign;
	char	*name;
	char	*value;

	expanded = expand_variables(input, data->envp, data);
	if (!expanded)
		return (perror("Error"), false);
	equal_sign = ft_strchr(expanded, '=');
	if (!equal_sign)
		return (free(expanded), false);
	*equal_sign = '\0';
	name = expanded;
	value = equal_sign + 1;
	process_environment_variable(name, value, data);
	process_user_variable(name, value, env_vars, data);
	free(expanded);
	return (true);
}
