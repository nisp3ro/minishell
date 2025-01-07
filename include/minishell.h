
#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/libft.h"
# include "colorsdef.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_vars
{
	char			*name;
	char			*value;
	struct s_vars	*next;
}					t_vars;

typedef struct s_data
{
	char			**envp;
	//char    **cmd;
	char			*home;
	char			*pwd;
	char			*oldpwd;
	char			*prompt;
	t_vars			*vars;
}					t_data;

//signals.c
void    handle_ctrl_c(int sig);
void    init_signal(void);

//init_data.c
char    **cpy_env(char *envp[]);
void    init_data(t_data *data, char *envp[]);

//utils.c
bool    is_all_spaces(char *str);
bool    complete_quotes(char **full_cmd);

//get_prompt.c
int    get_prompt(char **prompt, t_data *data);
char    *mini_getenv(char *var, char *envp[]);

//vars.c
void    expand_variables(char **full_cmd, char *envp[], t_data *data);
t_vars  *ft_lstnewvar(char *content);
char	*mini_getvars(char *var, t_vars *vars);

#endif
