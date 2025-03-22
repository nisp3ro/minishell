#include "../../include/minishell.h"

/**
 * @brief Reads the first line from the system environment file.
 *
 * Opens the "/etc/environment" file in read-only mode and retrieves the first line
 * using get_next_line(). It then reads and frees any subsequent lines before closing
 * the file. This function is typically used to extract the PATH variable from the system
 * environment when no environment is provided to the shell.
 *
 * @return char* The first line read from "/etc/environment", or NULL on error.
 */
static char	*get_path_line(void)
{
	int		fd;
	char	*line;
	char	*tmp;

	fd = open("/etc/environment", O_RDONLY);
	if (fd < 0)
		return (NULL);
	line = get_next_line(fd);
	tmp = get_next_line(fd);
	while (tmp)
	{
		free(tmp);
		tmp = get_next_line(fd);
	}
	close(fd);
	return (line);
}

/**
 * @brief Creates a PATH variable from the system environment when none is provided.
 *
 * This function checks if the file "/etc/environment" exists. If it does, it reads
 * the first line from that file using get_path_line(), then processes the line by:
 * - Removing any newline character.
 * - Splitting the line at the '=' character.
 * - Trimming double quotes from the remainder.
 * The processed value is then set as a variable in the shell's variable list.
 *
 * @param data Pointer to the minishell data structure.
 * @return int Returns OK on success or ERROR on failure.
 *
 * @note There is potential risk if the '=' character is not found in the line,
 *       as the function then attempts to use a NULL pointer with ft_strtrim.
 */
int	create_path_no_env(t_data *data)
{
	char	*tmp;
	char	*line;

	if (access("/etc/environment", F_OK) == 0)
	{
		line = get_path_line();
		if (line)
		{
			tmp = NULL;
			tmp = ft_strchr(line, '\n');
			if (tmp)
				*tmp = '\0';
			tmp = NULL;
			tmp = ft_strchr(line, '=');
			if (tmp)
				*tmp = '\0';
			tmp = ft_strtrim(tmp + 1, "\"");
			if (tmp == NULL)
				return (free(line), ERROR);
			if (set_variable(&data->vars, line, tmp) == ERROR)
				return (free(line), free(tmp), ERROR);
			return (free(line), free(tmp), OK);
		}
	}
	return (OK);
}

/**
 * @brief Updates the SHLVL (shell level) environment variable.
 *
 * This function iterates through the environment variable array in data->envp looking
 * for the "SHLVL=" variable. When found, it increments the numeric value, rebuilds the
 * variable string, and updates data->envp accordingly. If "SHLVL=" is not found, it adds
 * it with a default value of "1".
 *
 * @param data Pointer to the minishell data structure.
 * @return int Returns OK on success or ERROR on failure.
 */
int	update_envp(t_data *data)
{
	int		i;
	int		lvl;
	char	*tmp;

	i = -1;
	tmp = NULL;
	while (data->envp[++i])
	{
		if (ft_strncmp(data->envp[i], "SHLVL=", 6) == 0)
		{
			lvl = ft_atoi(data->envp[i] + 6);
			free(data->envp[i]);
			tmp = ft_itoa(++lvl);
			if (!tmp)
				return (ERROR);
			data->envp[i] = ft_strjoin("SHLVL=", tmp);
			if (!data->envp[i])
				return (free(tmp), ERROR);
			return (free(tmp), OK);
		}
	}
	if (!data->envp[i])
		if (set_exp(data, "SHLVL", "1") == ERROR)
			return (ERROR);
	return (OK);
}

/**
 * @brief Creates a copy of the environment variable array.
 *
 * This function allocates a new array of strings and duplicates each string from the
 * provided envp array. If any allocation fails, all previously allocated memory is freed.
 *
 * @param envp The original environment variable array.
 * @return char** A pointer to the newly allocated environment copy, or NULL on error.
 */
char	**cpy_env(char *envp[])
{
	char	**cpy;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	cpy = malloc(sizeof(char *) * (i + 1));
	if (cpy == NULL)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		cpy[i] = ft_strdup(envp[i]);
		if (!cpy[i])
		{
			while (i >= 0)
				free(cpy[i--]);
			free(cpy);
			return (NULL);
		}
		i++;
	}
	cpy[i] = NULL;
	return (cpy);
}

/**
 * @brief Creates a minimal environment if none is provided.
 *
 * This function allocates a new environment variable array with a default set of variables:
 * - PWD: current working directory.
 * - SHLVL: shell level, initially set to " 1" (note the space).
 * - _: a placeholder variable.
 * If any allocation fails, the function cleans up and returns ERROR.
 *
 * @param data Pointer to the minishell data structure.
 * @return int Returns OK on success or ERROR on failure.
 */
int	create_envp(t_data *data)
{
	char	*tmp_pwd;
	char	*tmp_cwd;

	data->envp = malloc(sizeof(char *) * 4);
	if (data->envp == NULL)
		return (clean_mtx(data->envp), ERROR);
	tmp_pwd = ft_strdup("PWD=");
	if (tmp_pwd == NULL)
		return (clean_mtx(data->envp), ERROR);
	tmp_cwd = getcwd(NULL, 0);
	if (tmp_cwd == NULL)
		return (free(tmp_pwd), clean_mtx(data->envp), ERROR);
	data->envp[0] = ft_strjoin(tmp_pwd, tmp_cwd);
	free(tmp_pwd);
	free(tmp_cwd);
	if (data->envp[0] == NULL)
		return (clean_mtx(data->envp), ERROR);
	data->envp[1] = ft_strdup("SHLVL= 1");
	if (data->envp[1] == NULL)
		return (clean_mtx(data->envp), ERROR);
	data->envp[2] = ft_strdup("_=]");
	if (data->envp[2] == NULL)
		return (clean_mtx(data->envp), ERROR);
	data->envp[3] = NULL;
	return (OK);
}
