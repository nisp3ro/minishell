#include "../include/minishell.h"

int g_error;

int	interactive_mode(t_data *data, char *envp[])
{
	char		*line;
	char		*full_cmd;
	t_token		*tokens;
	t_command	*commands;
	char		*tmp;
	char		*tmp2;
	int			i;

	i = 0;
	while (1)
	{
		if (get_prompt(&data->prompt, data) == ERROR)
			return (ERROR);
		line = readline(data->prompt);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		while (isspace((unsigned char)line[i]))
			i++;
		if (line[i] == '|')
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			g_error = 258;
			free(line);
			continue ;
		}
		while (1)
		{
			i = ft_strlen(line) - 1;
			while (isspace((unsigned char)line[i]))
				i--;
			if (line[i] == '|')
			{
				tmp = readline(">");
				tmp2 = ft_strjoin(line, " ");
				free(line);
				line = ft_strjoin(tmp2, tmp);
				free(tmp);
				free(tmp2);
			}
			else
				break ;
		}
		if (line[0] != '\0' && !is_all_spaces(line))
			add_history(line);
		full_cmd = ft_strtrim(line, " ");
		free(line);
		if (full_cmd == NULL)
			return (perror("Error"), ERROR);
		if (ft_strcmp(full_cmd, "exit") == 0 || ft_strncmp(full_cmd, "exit ",
				5) == 0)
		// esto es un builtin
		{
			free(data->prompt);
			free(full_cmd);
			break ;
		}
		tmp = ft_strchr(full_cmd, '=');
		if (tmp && *(tmp + 1) && *(tmp + 1) != ' ' && *(tmp + 1) != '=')
			handle_variable_assignment(full_cmd, &data->vars);
		else if (full_cmd[0] != '\0')
		{
			tokens = tokenize(full_cmd, envp, data);
			commands = parse_pipeline(tokens);
			commands->isfather = true;
			if (!commands->next && check_builtin(commands, data) == true)
			{
				free(data->prompt);
				free(full_cmd);
				continue ;
			}
			else
			{
				commands->isfather = false;
				execute_pipeline(commands, data, data->envp);
			}
		}
		free(data->prompt);
		free(full_cmd);
	}
	return (OK);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_data data;

	g_error = 0;
	if (argc == 2 || (argc > 2 && ft_strcmp(argv[1], "-c") != 0))
		return (printf("You are doing it wrong!"), 127); // mensaje de error
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
		t_token *tokens = tokenize(argv[2], envp, &data);
		t_command *commands = parse_pipeline(tokens);
		if (!commands->next && check_builtin(commands, &data) == true)
			return (OK); // return(limpiar, 0)
		execute_pipeline(commands, &data, data.envp);
	}
	return (OK); // return(limpiar, 0)
}