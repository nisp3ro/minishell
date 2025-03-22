#include "../../include/minishell.h"

/**
 * @brief Adds a new variable to the environment variable list.
 *
 * This function allocates a new t_vars node, duplicates the provided name and value,
 * and prepends it to the linked list of environment variables pointed to by env_vars.
 *
 * @param env_vars Double pointer to the head of the environment variable list.
 * @param name The name of the variable to add.
 * @param value The value of the variable to add.
 * @return int Returns OK if the variable was added successfully, or ERROR on failure.
 */
int	set_variable(t_vars **env_vars, char *name, char *value)
{
	t_vars	*new_var;

	new_var = malloc(sizeof(t_vars));
	if (new_var == NULL)
		return (ERROR);
	new_var->name = ft_strdup(name);
	if (new_var->name == NULL)
	{
		free(new_var);
		return (ERROR);
	}
	new_var->value = ft_strdup(value);
	if (new_var->value == NULL)
	{
		free(new_var->name);
		free(new_var);
		return (ERROR);
	}
	new_var->next = *env_vars;
	*env_vars = new_var;
	return (OK);
}
