/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:10:01 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/27 17:30:43 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
		g_exit_code = 1;
	}
	return ;
}

static void	here_doc_handler(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		if (kill(getpid(), SIGKILL) == -1)
		{
			perror("kill");
			g_exit_code = 130;
		}
	}
	return ;
}

static void	child_handler(int signal)
{
	if (signal == SIGINT)
		g_exit_code = 130;
	return ;
}

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
