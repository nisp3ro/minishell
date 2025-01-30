/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   git_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:14:11 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/30 20:01:51 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

	name = NULL;
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

char	*make_brach(char *branch)
{
	char	*tmp;

	if (ft_strncmp(branch, "ref: refs/heads/", 16) == 0)
	{
		tmp = branch;
		branch = ft_strdup(branch + 16);
		free(tmp);
		if (!branch)
			return (NULL);
		branch[ft_strlen(branch) - 1] = '\0';
	}
	else
	{
		tmp = branch;
		branch = ft_strdup(branch + 33);
		free(tmp);
		if (!branch)
			return (NULL);
		branch[ft_strlen(branch) - 1] = '\0';
	}
	return (branch);
}

char	*get_branch(const char *dir_path)
{
	char	*tmp;
	char	*branch;
	int		fd;

	tmp = ft_strjoin(dir_path, "/.git/HEAD");
	if (!tmp)
		return (NULL);
	fd = open(tmp, O_RDONLY);
	free(tmp);
	if (fd == -1)
		return (NULL);
	branch = NULL;
	branch = get_next_line(fd);
	tmp = get_next_line(fd);
	while (tmp)
	{
		free(branch);
		tmp = get_next_line(fd);
	}
	close(fd);
	if (!branch)
		return (NULL);
	branch = make_brach(branch);
	return (branch);
}

int	is_a_git(t_data *data, bool *git_found, char **name)
{
	char	*tmp;
	char	*parent_dir;
	char	*last_slash;

	parent_dir = NULL;
	last_slash = NULL;
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
