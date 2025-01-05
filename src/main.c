#include "../include/minishell.h"

/**
 * init_data - Initializes a `t_data` structure with environment data.
 */
void	init_data(t_data *data, char *env[])
{
	data->env = env;
	data->path = NULL;
	data->cmd = NULL;
	data->home = getenv("HOME");
	data->pwd = getenv("PWD");
	data->oldpwd = NULL;
}
/**
 * host_trim - Trims a hostname to exclude domain and directory information.
 *
 * This function processes a given hostname string to remove the domain part
 * (after the first dot) and any directory path (before the first slash). It
 * returns the trimmed portion of the hostname, which represents only the base
 * hostname without additional details.
 *
 * Parameters:
 *    - host: A pointer to the original hostname string.
 *
 * Return:
 *    - A dynamically allocated string containing the trimmed hostname,
 *      or NULL if memory allocation fails.
 *
 * Note:
 *    - The caller is responsible for freeing the allocated memory for the
 *      returned string.
 */
char	*host_trim(char *host)
{
	char	*trimmed;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	while (host[i] != '\0' && host[i++] != '/')
		j = i;
	while (host[++j] != '\0' && host[j] != '.')
		trimmed = malloc(sizeof(char) * (j - i + 1));
	if (!trimmed)
		return (NULL);
	k = 0;
	while (i < j)
		trimmed[k++] = host[i++];
	trimmed[k] = '\0';
	return (trimmed);
}
/**
 * get_prompt - Creates a custom shell prompt string.
 *
 * This function builds a prompt using the current user's name, host name,
 * current working directory, and color formatting. It retrieves environment
 * variables (USER, HOSTNAME, PWD,
	HOME) and uses default values if any are missing.
 * The prompt includes user@host in a specified color and the current directory
 * relative to the home directory.
 *
 * Return:
 *    - A dynamically allocated string with the formatted prompt or NULL if
 *      memory allocation fails.
 * Notes:
 *    - You must free the returned string when you are done with it.
 */
char	*get_prompt(void)
{
	char	*prompt;
	char	*home;
	char	*user;
	char	*host;
	char	*pwd;
	int		free_host;
	int		i;

	user = getenv("USER");
	if (!user)
		user = "unknown_user";
	host = getenv("HOSTNAME");
	free_host = 0;
	if (!host)
	{
		host = getenv("SESSION_MANAGER");
		host = host_trim(host);
		free_host = 1;
		if (!host)
		{
			host = "unknown_host";
			free_host = 0;
		}
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
		pwd = "/unknown_directory";
	home = getenv("HOME");
	if (!home)
		home = "";
	prompt = malloc(sizeof(char) * (strlen(user) + strlen(pwd) - strlen(home)
				+ 38));
	if (!prompt)
		return (NULL);
	strcpy(prompt, BBLUE);                    // cambiar a fcnt de ftlibft
	strcat(prompt, user);                     // cambiar a fcnt de ftlibft
	strcat(prompt, "@");                      // cambiar a fcnt de ftlibft
	strcat(prompt, host);                     // cambiar a fcnt de ftlibft
	strcat(prompt, RESET_COLOR BRIGHT_GREEN); // cambiar a fcnt de ftlibft
	strcat(prompt, " ~");                     // cambiar a fcnt de ftlibft
	i = strlen(home);
	strcat(prompt, pwd + i);     // cambiar a fcnt de ftlibft
	strcat(prompt, RESET_COLOR); // cambiar a fcnt de ftlibft
	// strcat(prompt, "\n"); //cambiar a fcnt de ftlibft
	strcat(prompt, "$ "); // cambiar a fcnt de ftlibft
	if (free_host)
		free(host);
	free(pwd);
	return (prompt);
}

int	main(int argc, char *argv[], char *env[])
{
	t_data data;
	char *prompt;
	char *full_cmd;
	init_signal();
	init_data(&data, env);
	while (1)
	{
		prompt = get_prompt();
		full_cmd = readline(prompt);

		if (full_cmd)
		{
			if ((size_t)full_cmd == EOF)
				exit(1);
			if (strcmp(full_cmd, "cd") == 0)
				chdir(getenv("HOME"));
			if (full_cmd[0] != '\0')
				add_history(full_cmd);
			// parsear!
			// ejecutar!
			free(prompt);
			printf("%s\n", full_cmd); // debug
			free(full_cmd);
		}
		else
		{
			printf("exit\n");
            break ;
			exit(1); // triggers with ctrl-d}
		}
	}
    return (0);
}