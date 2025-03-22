#include "../../include/minishell.h"

/**
 * @brief Handles input redirection by opening the specified file.
 * 
 * If an input redirection (`< file`) is found, this function opens the file 
 * in read-only mode and updates the file descriptor.
 * 
 * @param redir The redirection structure containing the filename.
 * @param fd_in Pointer to the input file descriptor.
 * @return true If the file was successfully opened.
 * @return false If there was an error opening the file.
 */
static bool	handle_input_redirection(t_redir *redir, int *fd_in)
{
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	*fd_in = open(redir->value, O_RDONLY);
	if (*fd_in < 0)
	{
		perror("open");
		return (false);
	}
	return (true);
}

/**
 * @brief Handles output redirection by opening/creating the specified file.
 * 
 * If an output redirection (`> file` or `>> file`) is found, this function opens 
 * or creates the file with the appropriate mode (overwrite or append).
 * 
 * @param redir The redirection structure containing the filename.
 * @param fd_out Pointer to the output file descriptor.
 * @param append If true, appends to the file (`>>`), otherwise overwrites (`>`).
 * @return true If the file was successfully opened/created.
 * @return false If there was an error opening/creating the file.
 */
static bool	handle_output_redirection(t_redir *redir, int *fd_out, bool append)
{
	if (*fd_out != STDOUT_FILENO)
		close(*fd_out);
	if (append)
		*fd_out = open(redir->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*fd_out = open(redir->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd_out < 0)
	{
		perror("open");
		return (false);
	}
	return (true);
}

/**
 * @brief Processes all redirections for a given command.
 * 
 * Iterates through the command's redirections and applies input/output handling.
 * 
 * @param command The command structure containing redirections.
 * @param fd_in_out Array of file descriptors for input [0] and output [1].
 * @param i_redir Pointer to a boolean indicating if input redirection was found.
 * @param o_redir Pointer to a boolean indicating if output redirection was found.
 * @return true If all redirections were processed successfully.
 * @return false If any redirection failed.
 */
static bool	process_redirections(t_command *command, int *fd_in_out,
		bool *i_redir, bool *o_redir)
{
	while (command->redir)
	{
		if (command->redir->type == INPUT)
		{
			*i_redir = true;
			if (!handle_input_redirection(command->redir, &fd_in_out[0]))
				return (false);
		}
		else if (command->redir->type == OUTPUT)
		{
			*o_redir = true;
			if (!handle_output_redirection(command->redir, &fd_in_out[1],
					command->append))
				return (false);
		}
		command->redir = command->redir->next;
	}
	return (true);
}

/**
 * @brief Checks and applies input/output redirections for a command.
 * 
 * This function processes redirections and applies them using `dup2()`, 
 * redirecting `stdin` and `stdout` as necessary.
 * 
 * @param command The command structure containing redirections.
 * @return true If all redirections were applied successfully.
 * @return false If any redirection failed.
 */
bool	check_redirs(t_command *command)
{
	int		fd_in_out[2];
	bool	input_redirection;
	bool	output_redirection;

	fd_in_out[0] = STDIN_FILENO;
	fd_in_out[1] = STDOUT_FILENO;
	input_redirection = false;
	output_redirection = false;
	if (!process_redirections(command, fd_in_out, &input_redirection,
			&output_redirection))
		return (false);
	if (input_redirection)
	{
		dup2(fd_in_out[0], STDIN_FILENO);
		close(fd_in_out[0]);
	}
	if (output_redirection)
	{
		dup2(fd_in_out[1], STDOUT_FILENO);
		close(fd_in_out[1]);
	}
	return (true);
}
