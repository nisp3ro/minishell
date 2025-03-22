#include "../../include/minishell.h"

/**
 * @brief Changes the current directory to the given path.
 * 
 * If the change fails, an error message is printed.
 * 
 * @param path The path to change to.
 * @param data The minishell data structure.
 * @return true If the directory change was successful.
 * @return false If the directory change failed.
 */
static bool	handle_cd_path(char *path, t_data *data)
{
	if (chdir(path) == -1)
	{
		perror("cd");
		data->g_exit_code = 1;
		return (false);
	}
	return (true);
}

/**
 * @brief Handles special cases for `cd`, like `cd ~`, `cd -`, and `cd` (without arguments).
 * 
 * - If no arguments are provided, it changes to `$HOME`.
 * - If `~` is used, it replaces it with `$HOME`.
 * - If `-` is used, it changes to the previous directory (`OLDPWD`).
 * 
 * @param command The command structure.
 * @param data The minishell data structure.
 * @return true If the directory was changed successfully.
 * @return false If there was an error.
 */
static bool	handle_special_paths(t_command *command, t_data *data)
{
	char	*home;
	char	*tmp;

	if (!command->args[1])
	{
		home = mini_getenv("HOME", data->envp);
		if (!home)
			return (perror("cd: HOME not set"), false);
		return (handle_cd_path(home, data));
	}
	else if (command->args[1][0] == '~')
	{
		home = mini_getenv("HOME", data->envp);
		if (!home)
			return (perror("cd: HOME not set"), false);
		tmp = ft_strjoin(home, command->args[1] + 1);
		if (!tmp || !handle_cd_path(tmp, data))
			return (free(tmp), false);
		free(tmp);
		return (true);
	}
	else if (ft_strncmp(command->args[1], "-", 2) == 0)
		return (handle_cd_path(data->oldpwd, data));
	return (handle_cd_path(command->args[1], data));
}

/**
 * @brief Updates the PWD and OLDPWD environment variables.
 * 
 * This function updates the internal `pwd` and `oldpwd` values
 * and modifies the environment variables accordingly.
 * 
 * @param data The minishell data structure.
 */
static void	update_pwd_and_oldpwd(t_data *data)
{
	char	*current_pwd;

	current_pwd = getcwd(NULL, 0);
	if (!current_pwd)
	{
		perror("getcwd");
		data->g_exit_code = 1;
		return ;
	}
	if (data->oldpwd)
		free(data->oldpwd);
	data->oldpwd = data->pwd;
	update_env_var(data, "OLDPWD", data->oldpwd);
	data->pwd = current_pwd;
	update_env_var(data, "PWD", data->pwd);
}

/**
 * @brief Implements the `cd` built-in command.
 * 
 * - Handles argument errors.
 * - Calls `handle_special_paths` for special cases.
 * - Updates PWD and OLDPWD if the directory change is successful.
 * 
 * @param command The command structure.
 * @param data The minishell data structure.
 */
void	ft_cd(t_command *command, t_data *data)
{
	if (command->args[2])
	{
		write(STDERR_FILENO, "cd: too many arguments\n", 23);
		data->g_exit_code = 1;
		return ;
	}
	if (!handle_special_paths(command, data))
		return ;
	update_pwd_and_oldpwd(data);
	data->g_exit_code = 0;
}
