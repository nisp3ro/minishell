#include "../../include/minishell.h"

/**
 * @brief Handles SIGINT in the main process.
 *
 * This signal handler is used for interactive input.
 * When SIGINT is received, it clears the current readline input, prints a newline,
 * and resets the prompt line.
 *
 * @param signal The signal number received.
 */
static void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	return ;
}

/**
 * @brief Handles SIGINT during a here-document session.
 *
 * This signal handler is set during here-document processing.
 * When SIGINT is received, it prints a newline and forcefully terminates the process
 * by sending SIGKILL to itself.
 *
 * @param signal The signal number received.
 */
static void	here_doc_handler(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		if (kill(getpid(), SIGKILL) == -1)
		{
			perror("kill");
		}
	}
	return ;
}

/**
 * @brief Handles SIGINT in a child process.
 *
 * This signal handler is used in child processes to simply print a newline
 * when SIGINT is received.
 *
 * @param signal The signal number received.
 */
static void	child_handler(int signal)
{
	if (signal == SIGINT)
		write(1, "\n", 1);
	return ;
}

/**
 * @brief Configures signal handling based on the provided mode.
 *
 * This function sets up the signal handler for SIGINT and ignores SIGQUIT.
 * The handler for SIGINT is selected based on the parameter:
 * - i == 1: Uses signal_handler (for the main process).
 * - i == 0: Uses child_handler (for child processes).
 * - i == 2: Uses here_doc_handler (for here-document processing).
 * The SA_RESTART flag is set so that system calls are restarted if interrupted.
 *
 * @param i An integer flag indicating which signal handler to install:
 *          1 for the main process, 0 for child processes, 2 for here-documents.
 */
void	wait_signal(int i)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	signal(SIGQUIT, SIG_IGN);
	if (i == 1)
		sa.sa_handler = &signal_handler;
	else if (i == 0)
		sa.sa_handler = &child_handler;
	else if (i == 2)
		sa.sa_handler = &here_doc_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
}
