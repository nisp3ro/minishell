#include "../include/minishell.h"

extern int g_error;

char	*mini_getvars(t_vars *vars, const char *name)
{
	t_vars *current = vars; // Initialize current to the head of the list
	while (current != NULL)
	{
		if (current->name && strcmp(current->name, name) == 0)
		{
			return (current->value); // Variable found
		}
		current = current->next;
	}
	return (NULL); // Return NULL if the variable is not found
}

void	handle_variable_assignment(char *input, t_vars **env_vars)
{
	char	*equal_sign;
	char	*name;
	char	*value;
	char	*tmp;
	bool	in_simple_quotes;
	bool	in_double_quotes;
	int		i;

	i = 0;
	in_simple_quotes = false;
	in_double_quotes = false;
	equal_sign = ft_strchr(input, '=');
	*equal_sign = '\0';
	equal_sign++;
	name = input;
	tmp = equal_sign;
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
	value = (char *)malloc(sizeof(char) * (i + 1));
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
	set_variable(env_vars, name, value);
}

void	set_variable(t_vars **env_vars, char *name, char *value)
{
	t_vars	*new_var;

	new_var = malloc(sizeof(t_vars));
	if (!new_var)
	{
		perror("malloc");
		return ;
	}
	new_var->name = ft_strdup(name);
	new_var->value = value;
	new_var->exported = false;
	new_var->next = *env_vars;
	*env_vars = new_var;
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
		if (token_value[i] == '$' && ft_isalnum(token_value[i + 1]))
		{
			j = i + 1;
			while (token_value[j] != '\0' && ft_isalnum(token_value[j]))
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
            tmp = ft_itoa(g_error);
            if (!tmp)
            {
                perror("Error");
                exit(1);  // En caso de error, salir con código de error
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
