/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 18:51:06 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/18 16:45:06 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*host_trim(char *host)
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

bool	has_git_directory(const char *dir_path)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(dir_path);
	if (dir == NULL)
	{
		return (false);
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (ft_strncmp(entry->d_name, ".git", 5) == 0)
		{
			closedir(dir);
			return (true);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (false);
}

int	is_a_daddy(char *parent_dir, t_data *data, bool *git_found,
		char *last_slash)
{
	parent_dir = ft_strdup(data->pwd);
	if (!parent_dir)
		return (ERROR);
	while (parent_dir && *parent_dir)
	{
		if (has_git_directory(parent_dir))
		{
			*git_found = true;
			break ;
		}
		last_slash = strrchr(parent_dir, '/');
		if (last_slash)
			*last_slash = '\0';
		else
			break ;
	}
	free(parent_dir);
	return (OK);
}

int	is_a_git(t_data *data, bool *git_found)
{
	char	*tmp;
	char	*parent_dir;
	char	*last_slash;

	*git_found = false;
	tmp = ft_strjoin(data->pwd, "/.git");
	if (!tmp)
		return (ERROR);
	*git_found = false;
	if (access(tmp, F_OK) == 0)
		*git_found = true;
	else
	{
		if (is_a_daddy(parent_dir, data, git_found, last_slash) == ERROR)
			return (free(tmp), ERROR);
	}
	free(tmp);
	return (OK);
}

int	print_prompt(char *prompt, char *user, char *host, t_data *data)
{
	bool	git_found;

	ft_strcpy(prompt, user);
	ft_strcat(prompt, "@");
	ft_strcat(prompt, host);
	ft_strcat(prompt, ":" BRIGHT_CYAN "~");
	if (is_a_git(data, &git_found) == ERROR)
		return (ERROR);
	ft_strcat(prompt, data->pwd + ft_strlen(data->home));
	if (git_found)
	{
		ft_strcat(prompt, BRIGHT_BLUE);
		ft_strcat(prompt, " git:(" BRIGHT_RED "main" BRIGHT_BLUE ")");
		ft_strcat(prompt, RESET_COLOR);
	}
	ft_strcat(prompt, RESET_COLOR);
	ft_strcat(prompt, "$ ");
	return (OK);
}

int	get_prompt(char **p, t_data *data)
{
	char	*user;
	char	*host;
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
	print_prompt(prompt, user, host, data);
	*p = prompt;
	if (free_host)
		free(host);
	return (OK);
}
