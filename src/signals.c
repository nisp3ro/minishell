#include "../include/minishell.h"

void	handle_ctrl_c(int sig)
{
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	init_signal(t_data *data)
{
	if (signal(SIGINT, handle_ctrl_c) == SIG_ERR)
	{
		int i;

		i = 0;
		while (data->envp[i])
			free(data->envp[i++]);
		free(data->envp);
		perror("SIGINT");
		exit(1);
	}
}