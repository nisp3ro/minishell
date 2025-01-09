#include "../include/minishell.h"


int interactive_mode(t_data *data, char *envp[])
{
    char *line;
    char *full_cmd;
    t_token *tokens;
    t_command *commands;

    while (1)
    {
        if (get_prompt(&data->prompt, data) == ERROR)
            return (ERROR);
        line = readline(data->prompt);
        if (!line)
        {
            printf("exit\n");
            break;
        }
        if (line[0] != '\0' && !is_all_spaces(line))
            add_history(line);
        full_cmd = ft_strtrim(line, " ");
        free(line);
        if (full_cmd == NULL)  
            return (perror("Error"), ERROR);
        if (ft_strcmp(full_cmd, "exit") == 0 || ft_strncmp(full_cmd, "exit ", 5) == 0) //esto ees un builtin
        {
            free(data->prompt);
            free(full_cmd);
            break;
        }
        if (strchr(full_cmd, '='))
            handle_variable_assignment(full_cmd, &data->vars);
        else
        { 
            tokens = tokenize(full_cmd, envp, data);
            commands = parse_pipeline(tokens);
            execute_pipeline(commands, data->envp); //selec_exec
        }
        free(data->prompt);
        free(full_cmd);
    }
    return (OK);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_data data;

	if (argc == 2 || (argc > 2 && ft_strcmp(argv[1], "-c") != 0))
        return (printf("You are doing it wrong!"), 127); // mensaje de error
    if (init_data(&data, envp) == ERROR)
        return (perror("Error"), 1);
	init_signal(&data);
    if (argc == 1)
    {
        if (interactive_mode(&data, data.envp) == ERROR)
            return (ERROR); //return(limpiar, 1)
    }
    else
    {
        t_token *tokens = tokenize(argv[2], envp, &data);
        t_command *commands = parse_pipeline(tokens);
        execute_pipeline(commands, data.envp);
    }
    return(OK); //return(limpiar, 0)
}