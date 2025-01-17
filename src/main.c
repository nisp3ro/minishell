#include "../include/minishell.h"

int g_error;

void set_last_cmd_arg(t_data *data, char *name, char *value)
{
	int i;

	i = 0;
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], name, ft_strlen(name)) == 0 && data->envp[i][ft_strlen(name)] == '=')
		{
			free(data->envp[i]);
			data->envp[i] = malloc(ft_strlen(name) + ft_strlen(value) + 2);
			if (data->envp[i] == NULL)
			{
				perror("malloc");
				return ;
			}
			ft_strcpy(data->envp[i], name);
			ft_strcat(data->envp[i], "=");
			ft_strcat(data->envp[i], value);
			return ;
		}
		i++;
	}
	set_exp(data, name, value);
}

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
		else if (full_cmd[0] != '\0')
		{
			tokens = tokenize(full_cmd, data);
			commands = parse_pipeline(data, tokens);
			if(commands->args && !commands->next)
			{
				if (ft_strncmp(commands->args[0], "export", 7) == 0 && commands->args[1] == NULL)
					set_last_cmd_arg(data, "_", commands->args[0]);
				else if (ft_strncmp(commands->args[0], "export", 7) != 0)
				{
					i = 0;
					while (commands->args[i])
						i++;
					set_last_cmd_arg(data, "_", commands->args[i - 1]);
				}					
			}
			if (!commands || !commands->next && check_builtin_prepipe(commands, data) == true)
			{
				free(full_cmd);
				continue ;
			}
			else
				execute_pipeline(commands, data, data->envp);
		}
		free(full_cmd);
	}
	return (g_error);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_data data;

	g_error = 0;
	if (argc == 2 || (argc > 2 && ft_strncmp(argv[1], "-c", 3) != 0))
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
		t_token *tokens = tokenize(argv[2], &data);
		t_command *commands = parse_pipeline(&data, tokens);
		if (!commands->next && check_builtin(commands, &data) == true)
			return (OK); // return(limpiar, 0)
		execute_pipeline(commands, &data, data.envp);
	}
	return (g_error); // return(limpiar, 0)
}