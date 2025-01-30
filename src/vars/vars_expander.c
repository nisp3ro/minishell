/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars_expander.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:41:14 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/30 13:10:12 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*handle_variable_expansion(char *token_value, int *i,
		char **expanded, t_data *data)
{
	char	*var_val[2];
	char	*tmp;
	int		j;
	int		k;

	j = *i + 1;
	k = 0;
	while ((token_value[j] != '\0' && ft_isalnum(token_value[j]))
		|| (token_value[j] != '\0' && token_value[j] == '_'))
		j++;
	var_val[0] = malloc(sizeof(char) * (j - *i));
	if (!var_val[0])
		return (free(*expanded), NULL);
	while (*i < j)
		var_val[0][k++] = token_value[(*i)++];
	var_val[0][k] = '\0';
	var_val[1] = mini_getenv(var_val[0] + 1, data->envp);
	if (!var_val[1])
		var_val[1] = mini_getvars(data->vars, var_val[0] + 1);
	if (!var_val[1])
		var_val[1] = "";
	tmp = ft_strjoin(*expanded, var_val[1]);
	free(*expanded);
	*expanded = tmp;
	return (free(var_val[0]), *expanded);
}

static char	*handle_exit_code_expansion(t_data *data, int *i, char **expanded)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_itoa(data->g_exit_code);
	if (!tmp)
	{
		perror("Error");
		exit(1);
	}
	tmp2 = ft_strjoin(*expanded, tmp);
	free(tmp);
	free(*expanded);
	*expanded = tmp2;
	*i += 2;
	return (*expanded);
}

static char	*append_literal_char(char c, char **expanded)
{
	char	*tmp;
	char	*tmp2;

	tmp = malloc(sizeof(char) * 2);
	if (!tmp)
	{
		free(*expanded);
		return (NULL);
	}
	tmp[0] = c;
	tmp[1] = '\0';
	tmp2 = ft_strjoin(*expanded, tmp);
	free(tmp);
	free(*expanded);
	*expanded = tmp2;
	return (*expanded);
}

static int	process_token_character(char *token_value, int *i, char **expanded,
		t_data *data)
{
	if ((token_value[*i] == '$' && ft_isalnum(token_value[*i + 1]))
		|| (token_value[*i] == '$' && token_value[*i + 1] == '_'))
	{
		if (!handle_variable_expansion(token_value, i, expanded, data))
			return (ERROR);
	}
	else if (token_value[*i] == '$' && token_value[*i + 1] == '?')
	{
		if (!handle_exit_code_expansion(data, i, expanded))
			return (ERROR);
	}
	else
	{
		if (!append_literal_char(token_value[(*i)++], expanded))
			return (ERROR);
	}
	return (OK);
}

char	*expand_variables(char *token_value, t_data *data)
{
	char	*expanded;
	int		i;

	i = 0;
	if (!token_value)
		return (NULL);
	expanded = ft_calloc(sizeof(char), (ft_strlen(token_value) + 1));
	if (!expanded)
		return (NULL);
	while (token_value[i] != '\0')
	{
		if (process_token_character(token_value, &i, &expanded, data) == ERROR)
			return (NULL);
	}
	return (expanded);
}
// OJO check llib expan? 
// (puese este comentario en la linea 120 y no recuerdo que queria decir)
