#include "../include/minishell.h"

/**
 * @brief Removes environment variables from the envp array.
 *
 * Iterates through the command arguments and removes each matching environment 
 * variable from the data->envp array. A match is determined when the variable name 
 * in envp exactly matches the provided argument and is immediately followed by '='.
 * After freeing the matched element, subsequent elements are shifted to fill the gap.
 *
 * @param command Pointer to the command structure containing the unset arguments.
 * @param data Pointer to the minishell data structure.
 */
void	unset_from_envp(t_command *command, t_data *data)
{
	int	i;
	int	j;
	int	k;

	k = 0;
	if (!command->args[1])
		return ;
	while (command->args[++k])
	{
		i = -1;
		while (data->envp[++i] != NULL)
		{
			if (ft_strncmp(data->envp[i], command->args[k],
					ft_strlen(command->args[k])) == 0
				&& data->envp[i][ft_strlen(command->args[k])] == '=')
			{
				free(data->envp[i]);
				j = i - 1;
				while (data->envp[++j] != NULL)
					data->envp[j] = data->envp[j + 1];
				break ;
			}
		}
	}
	data->g_exit_code = 0;
}

/**
 * @brief Removes a variable from the variables linked list.
 *
 * Searches for a variable in the provided linked list whose name exactly matches 
 * the given argument. If found, the variable node is removed from the list and its 
 * associated memory is freed.
 *
 * @param arg The name of the variable to remove.
 * @param vars Pointer to the pointer to the head of the linked list of variables.
 */
void	unset_from_vars(char *arg, t_vars **vars)
{
	t_vars	*tmp;
	t_vars	*prev;

	tmp = *vars;
	prev = NULL;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, arg, ft_strlen(arg)) == 0
			&& tmp->name[ft_strlen(arg)] == '\0')
		{
			if (prev)
				prev->next = tmp->next;
			else
				*vars = tmp->next;
			return (free(tmp->name), free(tmp->value), free(tmp));
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

/**
 * @brief Implements the unset built-in command.
 *
 * Processes the unset command by removing variables from both the environment (envp) 
 * and the variables linked lists (vars and exp_vars). It first removes matching 
 * environment variables, then iterates through each argument to remove them from the 
 * internal variable lists. Finally, it checks for the "PWD" variable and sets a flag 
 * (unset_pwd) if it is present.
 *
 * @param command Pointer to the command structure containing the unset arguments.
 * @param data Pointer to the minishell data structure.
 */
void	ft_unset(t_command *command, t_data *data)
{
	int	i;

	i = 0;
	if (!command->args[1])
		return ;
	unset_from_envp(command, data);
	while (command->args[++i])
	{
		unset_from_vars(command->args[i], &data->vars);
		unset_from_vars(command->args[i], &data->exp_vars);
	}
	i = 0;
	while (command->args[++i])
	{
		if (ft_strncmp(command->args[i], "PWD", 3) == 0)
		{
			data->unset_pwd = true;
			break ;
		}
	}
	data->g_exit_code = 0;
}
