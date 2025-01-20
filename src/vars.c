/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:44:39 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 20:06:55 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_variable_assignment(char *input, t_vars **env_vars, t_data *data)
{
	char	*existing_var;
	char	*equal_sign;
	char	*name;
	char	*value;
	char	*tmp;
	char	*expanded;
	bool	in_simple_quotes;
	bool	in_double_quotes;
	bool	env;
	int		i;
	t_vars	*tmp2;

	existing_var = NULL;
	expanded = expand_variables(input, data->envp, data);
	i = 0;
	in_simple_quotes = false;
	in_double_quotes = false;
	equal_sign = ft_strchr(expanded, '=');
	*equal_sign = '\0';
	equal_sign++;
	name = expanded;
	tmp = equal_sign;
	env = false;
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], name, ft_strlen(name)) == 0
			&& data->envp[i][ft_strlen(name)] == '=')
		{
			free(data->envp[i]);
			data->envp[i] = malloc(ft_strlen(name) + ft_strlen(equal_sign));
			if (data->envp[i] == NULL)
			{
				perror("malloc");
				return ;
			}
			ft_strcpy(data->envp[i], name);
			ft_strcat(data->envp[i], "=");
			ft_strcat(data->envp[i], equal_sign);
			env = true;
			break ;
		}
		i++;
	}
	while (equal_sign[i] && (equal_sign[i] != ' ' || in_simple_quotes
			|| in_double_quotes))
	{
		if (equal_sign[i] == '\'' && !in_simple_quotes && !in_double_quotes)
		{
			equal_sign++;
			in_simple_quotes = true;
		}
		else if (equal_sign[i] == '\"' && !in_simple_quotes
				&& !in_double_quotes)
		{
			equal_sign++;
			in_double_quotes = true;
		}
		else if (equal_sign[i] == '\"' && in_double_quotes)
		{
			equal_sign++;
			in_double_quotes = false;
		}
		else if (equal_sign[i] == '\'' && in_simple_quotes)
		{
			equal_sign++;
			in_double_quotes = false;
		}
		else
			i++;
	}
	value = (char *)ft_calloc(sizeof(char), (i + 1));
	i = 0;
	while (tmp[i] && (tmp[i] != ' ' || in_simple_quotes || in_double_quotes))
	{
		if (tmp[i] == '\'' && !in_simple_quotes && !in_double_quotes)
		{
			tmp++;
			in_simple_quotes = true;
		}
		else if (tmp[i] == '\"' && !in_simple_quotes && !in_double_quotes)
		{
			tmp++;
			in_double_quotes = true;
		}
		else if (tmp[i] == '\"' && in_double_quotes)
		{
			tmp++;
			in_double_quotes = false;
		}
		else if (tmp[i] == '\'' && in_simple_quotes)
		{
			tmp++;
			in_double_quotes = false;
		}
		else
		{
			value[i] = tmp[i];
			i++;
		}
	}
	existing_var = mini_getvars(data->exp_vars, name);
	if (existing_var)
	{
		free(existing_var);
		existing_var = ft_strdup(value);
		if (env == false)
			if (set_exp(data, name, value) == ERROR)
				return ; //limpiar y ver y tal
	}
	else if (env == false)
	{
		existing_var = mini_getvars(data->vars, name);
		if (existing_var)
		{
			free(existing_var);
			existing_var = ft_strdup(value);
		}
		else
			set_variable(env_vars, name, value);
	}
}

int	set_variable(t_vars **env_vars, char *name, char *value)
{
	t_vars	*new_var;

	new_var = malloc(sizeof(t_vars));
	if (new_var == NULL)
		return (ERROR);
	new_var->name = ft_strdup(name);
	if (new_var->name == NULL)
	{
		free(new_var);
		return (ERROR);
	}
	new_var->value = ft_strdup(value);
	if (new_var->value == NULL)
	{
		free(new_var->name);
		free(new_var);
		return (ERROR);
	}
	new_var->next = *env_vars;
	*env_vars = new_var;
	return (OK);
}

char	*expand_variables(char *token_value, char *envp[], t_data *data)
{
	char	*expanded;
	char	*tmp;
	char	*tmp2;
	char	*var;
	char	*value;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	expanded = ft_calloc(sizeof(char), (ft_strlen(token_value) + 1));
	if (!expanded)
		return (NULL);
	while (token_value[i] != '\0')
	{
		if ((token_value[i] == '$' && ft_isalnum(token_value[i + 1]))
			|| (token_value[i] == '$' && token_value[i + 1] == '_'))
		{
			j = i + 1;
			while ((token_value[j] != '\0' && ft_isalnum(token_value[j]))
				|| (token_value[j] != '\0' && token_value[j] == '_'))
				j++;
			var = malloc(sizeof(char) * (j - i));
			if (!var)
			{
				free(expanded);
				return (NULL);
			}
			k = 0;
			while (i < j)
				var[k++] = token_value[i++];
			var[k] = '\0';
			value = mini_getenv(var + 1, envp);
			if (!value)
				value = mini_getvars(data->vars, var + 1);
			if (!value)
				value = "";
			tmp = ft_strjoin(expanded, value);
			free(expanded);
			expanded = tmp;
			free(var);
		}
		else if (token_value[i] == '$' && token_value[i + 1] == '?')
		{
			tmp = ft_itoa(g_exit_code);
			if (!tmp)
			{
				perror("Error");
				exit(1); // En caso de error, salir con código de error
			}
			tmp2 = ft_strjoin(expanded, tmp);
			free(tmp);
			free(expanded);
			expanded = tmp2;
			i += 2;
		}
		else
		{
			tmp = malloc(sizeof(char) * 2);
			if (!tmp)
			{
				free(expanded);
				return (NULL);
			}
			tmp[0] = token_value[i];
			tmp[1] = '\0';
			tmp2 = ft_strjoin(expanded, tmp);
			free(expanded);
			free(tmp);
			expanded = tmp2;
			i++;
		}
	}
	return (expanded);
}
