#include "../include/minishell.h"

/**
 * @brief Entry point for the minishell program.
 *
 * This main function initializes the minishell data structure, processes command line
 * arguments, sets up signal handling, and runs either the interactive shell or a single
 * command execution based on the provided arguments.
 *
 * - If the program is invoked with arguments other than "-c", it writes an error message
 *   to standard error and exits with code 127.
 * - If no arguments or the proper "-c" argument is provided, it initializes the shell's data
 *   with the provided environment. In interactive mode (argc == 1), it enters the interactive shell;
 *   otherwise, it tokenizes and executes the command provided as argv[2].
 * - Finally, it cleans up allocated resources and clears the readline history.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument strings.
 * @param envp The array of environment variable strings.
 * @return int The exit code of the minishell, based on the global exit code in the data structure.
 */
int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;

	data.g_exit_code = 0;
	if (argc == 2 || (argc > 2 && ft_strncmp(argv[1], "-c", 3) != 0))
		return (write(STDERR_FILENO, "Error: Argument is not -c.\n", 28), 127);
	if (init_data(&data, envp) == ERROR)
		return (perror("Error"), 1);
	wait_signal(1);
	if (argc == 1)
	{
		if (interactive_mode(&data, data.envp) == ERROR)
			return (ERROR);
	}
	else
		token_parsec_exec(argv[2], &data, false);
	clean_data(&data);
	rl_clear_history();
	return (data.g_exit_code);
}
