/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:12:21 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/25 12:29:05 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**ft_realloc(char **envp, int size)
{
	char	**new;
	int		i;

	i = 0;
	new = ft_calloc(sizeof(char *), (size + 1));
	if (new == NULL)
		return (clean_mtx(envp), perror("malloc"), NULL);
	while (envp && envp[i] != NULL && envp[i][0] != '\0')
	{
		new[i] = ft_strdup(envp[i]);
		if (new[i] == NULL)
			return (clean_mtx(envp), clean_mtx(new), perror("malloc"), NULL);
		i++;
	}
	new[i] = NULL;
	i = 0;
	clean_mtx(envp);
	return (new);
}

int	is_delimiter(char c)
{
	return (c == '\t' || c == ' ' || c == '|' || c == '<' || c == '>');
}

bool	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (true);
	return (false);
}

char	*mini_getenv(char *var, char *envp[])
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] == var[j] && envp[i][j] != '=')
			j++;
		if (envp[i][j] == '=' && var[j] == '\0')
			return (envp[i] + j + 1);
		i++;
	}
	return (NULL);
}

bool	is_all_spaces(char *str)
{
	while (*str)
	{
		if (!ft_isspace(*str))
			return (false);
		str++;
	}
	return (true);
}

char	*mini_getvars(t_vars *vars, const char *name)
{
	t_vars *current = vars; // Initialize current to the head of the list
	while (current != NULL)
	{
		if (current->name && ft_strncmp(current->name, name, (ft_strlen(name)
					+ 1)) == 0)
		{
			return (current->value); // Variable found
		}
		current = current->next;
	}
	return (NULL); // Return NULL if the variable is not found
}

bool	complete_quotes(char **full_cmd)
{
	int		i;
	bool	in_double_quotes;
	bool	in_single_quotes;

	i = -1;
	in_double_quotes = false;
	in_single_quotes = false;
	while ((*full_cmd)[++i] != '\0')
	{
		if ((*full_cmd)[i] == '\\' && !in_single_quotes)
			i++;
		else if ((*full_cmd)[i] == '"' && !in_single_quotes)
		{
			if (in_double_quotes)
				in_double_quotes = false;
			else
				in_double_quotes = true;
		}
		else if ((*full_cmd)[i] == '\'' && !in_double_quotes)
		{
			if (in_single_quotes)
				in_single_quotes = false;
			else
				in_single_quotes = true;
		}
	}
	return (!(in_double_quotes || in_single_quotes));
}

//NORMINETTE
void	sort_list(t_vars **head, t_vars *current)
{
	t_vars	*sorted;
	t_vars	*next;
	t_vars	*t;

	sorted = NULL;
	current = *head;
	while (current != NULL)
	{
		next = current->next;
		if (sorted == NULL || ft_strcmp(current->name, sorted->name) < 0)
		{
			current->next = sorted;
			sorted = current;
		}
		else
		{
			t = sorted;
			while (t->next && ft_strcmp(t->next->name, current->name) < 0)
				t = t->next;
			current->next = t->next;
			t->next = current;
		}
		current = next;
	}
	*head = sorted;
}
