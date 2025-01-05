#include "../include/minishell.h"

void	handle_ctrl_c(int sig)
{
	printf("\nCtrl+C detected! Custom handler invoked.\n");
	exit(1);
}

void	init_signal(void)
{
	if (signal(SIGINT, handle_ctrl_c) == SIG_ERR)
	{
		perror("Error setting up signal handler");
		exit(1);
	}
}
