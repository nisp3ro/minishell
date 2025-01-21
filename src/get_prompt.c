/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 18:51:06 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 13:15:34 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*host_trim(char *host)
{
	char	*trimmed;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	while (host[i] != '\0' && host[i++] != '/')
		j = i;
	while (host[++j] != '\0' && host[j] != '.')
		trimmed = malloc(sizeof(char) * (j - i + 1));
	if (!trimmed)
		return (NULL);
	k = 0;
	while (i < j)
		trimmed[k++] = host[i++];
	trimmed[k] = '\0';
	return (trimmed);
}

char	*get_host(char **envp, int *free_host)
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
		}
		*free_host = 1;
		if (!host)
		{
			host = "42madrid";
			*free_host = 0;
		}
	}
	return (host);
}

static int	print_prompt2(char *prompt, char *user, char *host, t_data *data)
{
	bool	git_found;
	char	*name;

	ft_strcpy(prompt, user);
	ft_strcat(prompt, "@");
	ft_strcat(prompt, host);
	ft_strcat(prompt, ":~");
	if (is_a_git(data, &git_found, &name) == ERROR)
		return (ERROR);
	if (ft_strncmp(data->pwd, data->home, ft_strlen(data->home)) == 0)
		ft_strcat(prompt, data->pwd + ft_strlen(data->home));
	else
		ft_strcat(prompt, data->pwd);
	if (git_found)
	{
		ft_strcat(prompt, " git:(");
		if (name)
		{
			ft_strcat(prompt, name);
			//free(name);
		}
		ft_strcat(prompt, ")");
	}
	ft_strcat(prompt, "$ ");
	return (OK);
}

static int	print_prompt(char *prompt, char *user, char *host, t_data *data)
{
	bool	git_found;
	char	*name;

	ft_strcpy(prompt, user);
	ft_strcat(prompt, "@");
	ft_strcat(prompt, host);
	ft_strcat(prompt, ":" BRIGHT_CYAN "~");
	if (is_a_git(data, &git_found, &name) == ERROR)
		return (ERROR);
	if (ft_strncmp(data->pwd, data->home, ft_strlen(data->home)) == 0)
		ft_strcat(prompt, data->pwd + ft_strlen(data->home));
	else
		ft_strcat(prompt, data->pwd);
	if (git_found)
	{
		ft_strcat(prompt, BRIGHT_BLUE);
		ft_strcat(prompt, " git:(" BRIGHT_RED);
		if (name)
		{
			ft_strcat(prompt, name);
			//free(name);
		}
		ft_strcat(prompt, BRIGHT_BLUE ")");
		ft_strcat(prompt, RESET_COLOR);
	}
	ft_strcat(prompt, YELLOW" ✗ ");
	ft_strcat(prompt, RESET_COLOR);
	return (OK);
}

int	get_prompt(char **p, t_data *data)
{
	char	*user;
	char	*host;
	char	**tmp;
	int		free_host;
	char	prompt[PATH_MAX];

	data->home = mini_getenv("HOME", data->envp);
	user = mini_getenv("USER", data->envp);
	if (!user)
		user = "minishell";
	free_host = 0;
	host = get_host(data->envp, &free_host);
	if (host == NULL)
		return (ERROR);
	tmp = data->envp;
	while (*tmp)
	{
		if (ft_strnstr(*tmp, "COLOR", ft_strlen(*tmp)) != NULL)
		{
			print_prompt(prompt, user, host, data);
			break ;
		}
		else
			print_prompt2(prompt, user, host, data);
		tmp++;
	}
	*p = prompt;
	if (free_host)
		free(host);
	return (OK);
}
