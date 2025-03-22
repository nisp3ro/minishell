#include "../../include/minishell.h"

/**
 * @brief Allocates and initializes a new command structure.
 *
 * This function allocates memory for a new t_command structure and initializes its members
 * to default values:
 * - args, eof, redir, next are set to NULL.
 * - eof_count and arg_count are set to 0.
 * - append is set to 0.
 * - export is set to false.
 * - first is set to true.
 *
 * @return t_command* Pointer to the newly allocated and initialized command structure,
 * or NULL if memory allocation fails.
 */
t_command	*initialize_command(void)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->args = NULL;
	command->eof_count = 0;
	command->eof = NULL;
	command->append = 0;
	command->arg_count = 0;
	command->export = false;
	command->first = true;
	command->redir = NULL;
	command->next = NULL;
	return (command);
}
