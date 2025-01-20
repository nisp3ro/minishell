/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 11:21:20 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 17:16:30 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_error;

int	main(int argc, char *argv[], char *envp[])
{
	t_data		data;
	t_token		*tokens;
	t_command	*commands;

	g_error = 0;
	if (argc == 2 || (argc > 2 && ft_strncmp(argv[1], "-c", 3) != 0))
		return (write(STDERR_FILENO, "Error: Argument is not -c.\n", 28), 127);
	if (init_data(&data, envp) == ERROR)
		return (perror("Error"), 1);
	wait_signal(1);
	if (argc == 1)
	{
		if (interactive_mode(&data, data.envp) == ERROR)
			return (ERROR); // return(limpiar, 1)
	}
	else
	{
		tokens = tokenize(argv[2], &data);
		commands = parse_pipeline(&data, tokens);
		if (!commands->next && check_builtin(commands, &data) == true)
			return (OK); // return(limpiar, 0)
		execute_pipeline(commands, &data, data.envp);
	}
	rl_clear_history();
	return (g_error); // return(limpiar, 0)
}
