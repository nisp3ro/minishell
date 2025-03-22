#include "../../include/minishell.h"

/**
 * @brief Checks if a directory contains a .git directory.
 *
 * Opens the directory specified by dir_path and iterates through its entries.
 * If an entry with the name ".git" is found, the directory is closed and true is returned.
 * Otherwise, it returns false after closing the directory.
 *
 * @param dir_path The path to the directory to check.
 * @return true if the directory contains a ".git" subdirectory, false otherwise.
 */
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

/**
 * @brief Searches upward from the current working directory for a Git repository.
 *
 * This function duplicates the current working directory from data->pwd into parent_dir,
 * then iteratively checks if the directory contains a ".git" directory using has_git_directory().
 * If found, it retrieves the current branch using get_branch(), sets *git_found to true,
 * and returns the branch name. Otherwise, it truncates parent_dir to its parent directory
 * (using strrchr to find the last '/') and continues searching until none is left.
 *
 * @param parent_dir Unused input parameter (will be overwritten by a duplicate of data->pwd).
 * @param data Pointer to the minishell data structure.
 * @param git_found Pointer to a boolean flag that is set to true if a Git repository is found.
 * @param last_slash A pointer used to locate the last '/' in the path.
 * @return char* The Git branch name if found, or NULL if no Git repository is found.
 */
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

/**
 * @brief Formats a Git HEAD reference to extract the branch name.
 *
 * If the branch string starts with "ref: refs/heads/", it duplicates the substring
 * that follows and removes the trailing newline. Otherwise, it assumes an alternative format
 * and duplicates the substring starting at an offset of 33 characters, then removes the trailing newline.
 *
 * @param branch The original branch string read from .git/HEAD.
 * @return char* The formatted branch name, or NULL on allocation failure.
 */
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

/**
 * @brief Retrieves the current Git branch from a repository.
 *
 * Constructs the path to the .git/HEAD file in the directory specified by dir_path,
 * opens the file, and reads the first line to get the branch reference.
 * It then calls make_brach() to format the branch name.
 *
 * @param dir_path The path to the directory containing the .git folder.
 * @return char* The Git branch name if successfully read and formatted, or NULL on failure.
 */
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

/**
 * @brief Determines if the current working directory is part of a Git repository.
 *
 * Checks if the current directory (data->pwd) has a .git folder by constructing the path.
 * If the .git folder exists, it retrieves the current branch via get_branch() and sets
 * the git_found flag to true. If not, it calls is_a_daddy() to search upward in the directory
 * hierarchy for a Git repository.
 *
 * @param data Pointer to the minishell data structure.
 * @param git_found Pointer to a boolean flag that will be set to true if a Git repository is found.
 * @param name Pointer to a string that will receive the Git branch name if found.
 * @return int Returns OK on success, or ERROR on failure.
 */
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
		if (!(*name))
			return (free(tmp), ERROR);
	}
	free(tmp);
	return (OK);
}
