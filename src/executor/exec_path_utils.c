#include "../../include/minishell.h"

/**
 * @brief Retrieves the PATH variable from the environment.
 *
 * This function attempts to get the "PATH" variable from the provided envp array.
 * If it is not found there, it looks for it in the minishell variables (data->vars).
 *
 * @param data Pointer to the minishell data structure.
 * @param envp The environment variable array.
 * @return char* Pointer to the PATH string, or NULL if not found.
 */
static char	*get_path_var(t_data *data, char **envp)
{
	char	*path;

	path = mini_getenv("PATH", envp);
	if (!path)
		path = mini_getvars(data->vars, "PATH");
	return (path);
}

/**
 * @brief Finds the full path of a command using the PATH variable.
 *
 * This function splits the PATH variable into directories, then iterates through each
 * directory, constructing a full path by appending "/" and the command name. It checks
 * if the constructed path is executable using access(). If an executable is found, it
 * returns the full path. Otherwise, it frees the allocated memory and returns NULL.
 *
 * @param data Pointer to the minishell data structure.
 * @param command The command name to search for.
 * @param envp The environment variable array.
 * @return char* The full path to the command if found, or NULL otherwise.
 */
char	*find_command_in_path(t_data *data, char *command, char **envp)
{
	t_search_command	vars;

	vars.path = get_path_var(data, envp);
	if (!vars.path)
		return (NULL);
	vars.directories = ft_split(vars.path, ':');
	if (!vars.directories)
		return (NULL);
	init_search_command_vars(&vars);
	while (vars.directories[vars.i])
	{
		vars.command_path = ft_strjoin(vars.directories[vars.i], "/");
		if (!vars.command_path)
			return (clean_mtx(vars.directories), NULL);
		vars.tmp = ft_strjoin(vars.command_path, command);
		free(vars.command_path);
		vars.command_path = vars.tmp;
		if (access(vars.command_path, X_OK) == 0)
			return (clean_mtx(vars.directories), vars.command_path);
		free(vars.command_path);
		vars.command_path = NULL;
		vars.i++;
	}
	clean_mtx(vars.directories);
	return (NULL);
}
