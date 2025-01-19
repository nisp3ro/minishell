/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 18:51:06 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/19 14:17:56 by mrubal-c         ###   ########.fr       */
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

char	*is_a_daddy(char *parent_dir, t_data *data, bool *git_found,
		char *last_slash)
{
	char	*name;

	parent_dir = ft_strdup(data->pwd);
	if (!parent_dir)
		return (NULL);
	while (parent_dir && *parent_dir)
	{
		if (has_git_directory(parent_dir))
		{
			name = get_branch(parent_dir);
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
	return (name);
}

char	*get_branch(const char *dir_path) //limpiartodo
{
	char *tmp;
	char *branch;
	char *for_clean;
	int fd;

	tmp = ft_strjoin(dir_path, "/.git/HEAD");
	if (!tmp)
		return (NULL);
	fd = open(tmp, O_RDONLY);
	free(tmp);
	if (fd == -1)
		return (NULL);
	branch = NULL;
	branch = get_next_line(fd);
	for_clean = get_next_line(fd);
	//miar si tiene perdidas pondria un whiule por si acaso
	close(fd);
	if (!branch)
		return (NULL);
	if (ft_strncmp(branch, "ref: refs/heads/", 16) == 0)
	{
		tmp = branch;
		branch = ft_strdup(branch + 16);   //prteger
		branch = ft_strtrim(branch, "\n"); //tmbn
		free(tmp);
		if (!branch)
			return (NULL);
	}
	else
	{
		tmp = branch;
		branch = ft_strdup(branch + 33);   //proteger
		branch = ft_strtrim(branch, "\n"); //tmbn
		free(tmp);
	}
	return (branch);
}

int	is_a_git(t_data *data, bool *git_found, char **name)
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
	{
		*name = get_branch(data->pwd);
		*git_found = true;
	}
	else
	{
		*name = is_a_daddy(parent_dir, data, git_found, last_slash);
		if (!name)
			return (free(tmp), ERROR);
	}
	free(tmp);
	return (OK);
}

int	print_prompt2(char *prompt, char *user, char *host, t_data *data)
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

int	print_prompt(char *prompt, char *user, char *host, t_data *data)
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
	ft_strcat(prompt, RESET_COLOR);
	ft_strcat(prompt, "$ ");
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
			break;
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
