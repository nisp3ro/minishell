#include "../../include/minishell.h"

/**
 * @brief Removes a variable from the export list.
 *
 * Searches through the linked list of exported variables and removes the variable
 * whose name exactly matches the provided command string.
 *
 * @param command The name of the variable to remove.
 * @param vars Pointer to the head of the linked list of exported variables.
 */
static void	unset_from_vars_export(char *command, t_vars **vars)
{
	t_vars	*tmp;
	t_vars	*prev;

	tmp = *vars;
	prev = NULL;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, command, ft_strlen(command)) == 0
			&& tmp->name[ft_strlen(command)] == '\0')
		{
			if (prev)
				prev->next = tmp->next;
			else
				*vars = tmp->next;
			free(tmp->name);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

/**
 * @brief Exports a new variable that already exists in the environment.
 *
 * Retrieves the value of the variable from the environment and, if it is not already
 * present in the exported variables list, adds it. Afterwards, it sorts the export list.
 *
 * @param command The command structure containing the arguments.
 * @param data Pointer to the minishell data structure.
 * @param i The index of the variable in the command arguments.
 */
static void	export_new_variable(t_command *command, t_data *data, int i)
{
	char	*to_export;

	to_export = mini_getenv(command->args[i], data->envp);
	if (to_export && mini_getvars(data->exp_vars, command->args[i]) == NULL)
	{
		set_variable(&data->exp_vars, command->args[i], to_export);
		sort_list(&data->exp_vars, data->exp_vars);
	}
}

/**
 * @brief Exports an empty variable.
 *
 * Handles the case when the variable does not exist in either the variables list
 * or the exported variables list. It sets the exported variable with an empty value,
 * then sorts the export list.
 *
 * @param command The command structure containing the arguments.
 * @param data Pointer to the minishell data structure.
 * @param i The index of the variable in the command arguments.
 */
static void	export_empty_variable(t_command *command, t_data *data, int i)
{
	special_set_exp(data, command->args[i]);
	set_variable(&data->exp_vars, command->args[i], "");
	sort_list(&data->exp_vars, data->exp_vars);
}

/**
 * @brief Exports an existing variable.
 *
 * If the variable exists in the variables list, this function updates its export
 * status, moves it to the exported variables list, and sorts the export list. It also
 * removes the variable from the original variables list.
 *
 * @param command The command structure containing the arguments.
 * @param data Pointer to the minishell data structure.
 * @param i The index of the variable in the command arguments.
 */
static void	export_existing_variable(t_command *command, t_data *data, int i)
{
	char	*to_export;

	to_export = mini_getvars(data->vars, command->args[i]);
	if (set_exp(data, command->args[i], to_export) == ERROR)
		return ;
	set_variable(&data->exp_vars, command->args[i], to_export);
	unset_from_vars_export(command->args[i], &data->vars);
	sort_list(&data->exp_vars, data->exp_vars);
}

/**
 * @brief Implements the export built-in command.
 *
 * Processes each argument provided to the export command. Depending on whether
 * the variable already exists in the variables list, the exported variables list,
 * or neither, it calls the appropriate function to handle the export. If no arguments
 * are provided, it prints the list of exported variables.
 *
 * @param command The command structure containing the export command arguments.
 * @param data Pointer to the minishell data structure.
 */
void	ft_export(t_command *command, t_data *data)
{
	int	i;

	i = 1;
	data->g_exit_code = 0;
	while (command->args[i])
	{
		if (is_valid_identifier(command->args[i]) == ERROR)
		{
			handle_invalid_identifier(data, &i);
			continue ;
		}
		if (!mini_getvars(data->vars, command->args[i])
			&& !mini_getvars(data->exp_vars, command->args[i]))
			export_empty_variable(command, data, i);
		else if (mini_getvars(data->vars, command->args[i]))
			export_existing_variable(command, data, i);
		else
			export_new_variable(command, data, i);
		i++;
	}
	if (i == 1)
		print_exported_vars(data);
}
