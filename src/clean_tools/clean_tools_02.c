#include "../../include/minishell.h"

/**
 * @brief Frees memory allocated in the minishell data structure.
 *
 * This function cleans up the minishell data structure by freeing:
 * - The linked lists of variables (data->vars and data->exp_vars).
 * - The environment variables matrix (data->envp).
 * - The strings for the current working directory (data->pwd) and the previous directory (data->oldpwd).
 *
 * @param data Pointer to the minishell data structure.
 */
void	clean_data(t_data *data)
{
	clean_variables(data->vars);
	clean_variables(data->exp_vars);
	clean_mtx(data->envp);
	free(data->pwd);
	free(data->oldpwd);
}

/**
 * @brief Frees a linked list of redirections.
 *
 * Iterates through the redirection list and frees each node's value and the node itself.
 *
 * @param redir Pointer to the pointer to the head of the redirection list.
 */
void	clean_redir_list(t_redir **redir)
{
	t_redir	*tmp;

	while (*redir)
	{
		tmp = *redir;
		*redir = (*redir)->next;
		free(tmp->value);
		free(tmp);
	}
}

/**
 * @brief Frees a linked list of commands.
 *
 * Iterates through the command list and frees each command node, including:
 * - The arguments matrix (tmp->args) and the EOF matrix (tmp->eof) if they exist.
 * - The redirection list within the command.
 *
 * @param command Pointer to the first command in the linked list.
 */
void	clean_cmd(t_command *command)
{
	t_command	*tmp;
	t_redir		*redir_tmp;

	while (command)
	{
		tmp = command;
		command = command->next;
		if (tmp->args)
			clean_mtx(tmp->args);
		if (tmp->eof)
			clean_mtx(tmp->eof);
		while (tmp->redir)
		{
			redir_tmp = tmp->redir;
			tmp->redir = tmp->redir->next;
			free(redir_tmp->value);
			free(redir_tmp);
		}
		free(tmp);
	}
}
