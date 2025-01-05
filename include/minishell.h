#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/stat.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "colorsdef.h"


typedef struct s_data
{
    char	**env;
    char	**path;
    char    **cmd;
    char	*home;
    char	*pwd;
    char	*oldpwd;
}	t_data;

//signals.c
void	handle_ctrl_c(int sig);
void	init_signal(void);

#endif