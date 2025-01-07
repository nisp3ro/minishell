#include "../include/minishell.h"

void interactive_mode(t_data *data, char *envp[])
{
    char *line;
    char *full_cmd;

    while (1)
    {
        if (get_prompt(&data->prompt, data))
            break;
        line = readline(data->prompt);
        if (!line)
            break;
        if (line[0] != '\0' && !is_all_spaces(line))
            add_history(line);
        full_cmd = ft_strtrim(line, " ");
        while (!complete_quotes(&full_cmd))
        {
            char *tmp;
            char *tmp2;
            tmp2 = ft_strjoin(full_cmd, "\n");
            free(full_cmd);
            full_cmd = tmp2;
            tmp = readline("quote> ");
            if (!tmp)
                break;
            tmp2 = ft_strjoin(full_cmd, tmp);
            free(full_cmd);
            full_cmd = tmp2;
            free(tmp);
        }
        expand_variables(&full_cmd, envp, data);
        printf("%s\n", full_cmd);
        free(line);
        if (!full_cmd)
            break;
        if (ft_strcmp(full_cmd, "exit") == 0 || ft_strncmp(full_cmd, "exit ", 5) == 0)
        {
            free(data->prompt);
            free(full_cmd);
            break;
        }
        //parsear!
        //ejecutar!
        free(data->prompt);
        free(full_cmd);
    }
}

int	main(int argc, char *argv[], char *envp[])
{
	t_data data;

	if (argc != 1 && argc != 3 || argc == 3 && ft_strcmp(argv[1], "-c") != 0)
        return (printf("You are doing it wrong!"), 1); // mensaje de error
	init_signal();
	init_data(&data, envp);
    if (argc == 1)
        interactive_mode(&data, data.envp);
    if (argc == 3)
        printf("No interactive mode not programed yet.\n");
        //no_interactive_mode(&data, argv[2], data.envp);
    //limpiar bien!
    return (0);
}