/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 07:42:03 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/31 12:57:23 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*host_trim(char *host)
{
	char	*trimmed;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	while (host[i] != '\0' && host[i] != '/')
		i++;
	i++;
	j = i;
	while (host[j] != '\0' && host[j] != '.')
		j++;
	trimmed = malloc(sizeof(char) * (j - i + 1));
	if (!trimmed)
		return (NULL);
	while (host[i] && i < j)
		trimmed[k++] = host[i++];
	trimmed[k] = '\0';
	return (trimmed);
}

char	*get_host(char **envp, bool *free_host)
{
	char	*host;

	host = mini_getenv("HOST", envp);
	if (!host)
		host = mini_getenv("HOSTNAME", envp);
	if (!host)
	{
		host = mini_getenv("SESSION_MANAGER", envp);
		if (host)
		{
			host = host_trim(host);
			if (!host)
				return (NULL);
			*free_host = true;
		}
		if (!host)
		{
			host = "42madrid";
			*free_host = false;
		}
	}
	return (host);
}

static int	print_prompt(char *prompt, char *user, char *host, t_data *data)
{
	bool	git_found;
	char	*name;

	name = NULL;
	ft_strcpy(prompt, user);
	ft_strcat(prompt, "@");
	ft_strcat(prompt, host);
	ft_strcat(prompt, ":" BRIGHT_CYAN "~");
	if (ft_strncmp(data->pwd, data->home, ft_strlen(data->home)) == 0)
		ft_strcat(prompt, data->pwd + ft_strlen(data->home));
	else
		ft_strcat(prompt, data->pwd);
	if (is_a_git(data, &git_found, &name) == OK && git_found)
	{
		ft_strcat(prompt, BRIGHT_BLUE);
		ft_strcat(prompt, " git:(" BRIGHT_RED);
		if (name)
			ft_strcat(prompt, name);
		ft_strcat(prompt, BRIGHT_BLUE ")");
		ft_strcat(prompt, RESET_COLOR);
	}
	ft_strcat(prompt, YELLOW " ✗ ");
	ft_strcat(prompt, RESET_COLOR);
	return (free(name), OK);
}
//OJO COMPROBAR name siempre es algo o NULL antes de liberar en linea 90 ¿?

int	get_prompt(char **p, t_data *data)
{
	char	*user;
	char	*host;
	bool	free_host;
	char	prompt[PATH_MAX];

	data->home = mini_getenv("HOME", data->envp);
	user = mini_getenv("USER", data->envp);
	if (!user)
		user = "minishell";
	free_host = false;
	host = get_host(data->envp, &free_host);
	if (host == NULL)
		return (ERROR);
	print_prompt(prompt, user, host, data);
	*p = ft_strdup(prompt);
	if (free_host == true)
		free(host);
	return (OK);
}
