#include "../../include/minishell.h"

/**
 * @brief Reallocates and copies an environment variable array.
 * 
 * This function creates a new array of strings with an updated size and copies 
 * the existing environment variables into it.
 * 
 * @param envp The current environment variable array.
 * @param size The new size of the environment variable array.
 * @return char** A pointer to the new environment array, or NULL on failure.
 */
char	**ft_realloc(char **envp, int size)
{
	char	**new;
	int		i;

	i = 0;
	new = ft_calloc(sizeof(char *), (size + 1));
	if (new == NULL)
		return (clean_mtx(envp), perror("malloc"), NULL);
	while (envp && envp[i] != NULL && envp[i][0] != '\0')
	{
		new[i] = ft_strdup(envp[i]);
		if (new[i] == NULL)
			return (clean_mtx(envp), clean_mtx(new), perror("malloc"), NULL);
		i++;
	}
	new[i] = NULL;
	i = 0;
	clean_mtx(envp);
	return (new);
}

/**
 * @brief Retrieves the value of an environment variable.
 * 
 * This function searches for a given environment variable in the envp array.
 * 
 * @param var The name of the variable to find.
 * @param envp The environment variables array.
 * @return char* A pointer to the variable's value, or NULL if not found.
 */
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

/**
 * @brief Retrieves the value of a shell variable.
 * 
 * This function searches for a variable in a linked list of shell variables.
 * 
 * @param vars The linked list of shell variables.
 * @param name The name of the variable to find.
 * @return char* A pointer to the variable's value, or NULL if not found.
 */
char	*mini_getvars(t_vars *vars, const char *name)
{
	t_vars	*current;

	current = vars;
	while (current != NULL)
	{
		if (current->name && ft_strncmp(current->name, name, (ft_strlen(name)
					+ 1)) == 0)
		{
			return (current->value);
		}
		current = current->next;
	}
	return (NULL);
}

/**
 * @brief Sorts a linked list of shell variables alphabetically by name.
 * 
 * This function rearranges the linked list in alphabetical order using insertion sort.
 * 
 * @param head Pointer to the head of the linked list.
 * @param current The current node being processed.
 */
void	sort_list(t_vars **head, t_vars *current)
{
	t_vars	*sorted;
	t_vars	*next;
	t_vars	*t;

	sorted = NULL;
	current = *head;
	while (current != NULL)
	{
		next = current->next;
		if (sorted == NULL || ft_strcmp(current->name, sorted->name) < 0)
		{
			current->next = sorted;
			sorted = current;
		}
		else
		{
			t = sorted;
			while (t->next && ft_strcmp(t->next->name, current->name) < 0)
				t = t->next;
			current->next = t->next;
			t->next = current;
		}
		current = next;
	}
	*head = sorted;
}
