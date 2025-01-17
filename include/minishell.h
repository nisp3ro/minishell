/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:44:38 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/17 17:44:22 by mrubal-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/get_next_line.h"
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

# define ERROR -1
# define OK 0
# define PATH_MAX 4096

extern int				g_error;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND_OUT,
	TOKEN_HEREDOC,
	TOKEN_END,
}						t_token_type;

typedef enum e_redir_type
{
	INPUT,
	OUTPUT,

}						t_redir_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}						t_token;

typedef struct s_redir
{
	t_redir_type		type;
	char				*value;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	char				**args;
	char				*eof;
	int					append;
	t_redir				*redir;
	struct s_command	*next;
}						t_command;

typedef struct s_vars
{
	char				*name;
	char				*value;
	struct s_vars		*next;
}						t_vars;

typedef struct s_data
{
	char				**envp;
	char				*home;
	char				*pwd;
	char				*oldpwd;
	char				*prompt;
	t_vars				*vars;
	t_vars				*exp_vars;
}						t_data;

//signals.c
void					wait_signal(int i);

//init_data.c
char					**cpy_env(char *envp[]);
void					sort_list(t_vars **head, t_vars *current);
int						init_data(t_data *data, char *envp[]);

//utils.c
bool					is_all_spaces(char *str);
bool					complete_quotes(char **full_cmd);
void					sort_list(t_vars **head, t_vars *current);

//get_prompt.c
int						get_prompt(char **prompt, t_data *data);
char					*mini_getenv(char *var, char *envp[]);

//vars.c
char					*expand_variables(char *token_value, char *envp[],
							t_data *data);
char					*mini_getvars(t_vars *vars, const char *name);
void	handle_variable_assignment(char *input,
								t_vars **env_vars,
								t_data *data);
int						set_variable(t_vars **env_vars, char *name,
							char *value);

//tokenizer.c
int						is_delimiter(char c);
bool					is_quote(char c);
t_token					*add_token(t_token **tokens, t_token_type type,
							char *value);
t_token					*tokenize(char *input, t_data *data);

//parsing.c
t_command				*parse_tokens(t_data *data, t_token *tokens);
t_command				*parse_pipeline(t_data *data, t_token *tokens);

//exec.c
void					execute_pipeline(t_command *cmd, t_data *data,
							char **envp);
bool					ensure_directory_exists(const char *file_path);

//built_in.c
bool					check_builtin(t_command *command, t_data *data);
bool					check_builtin_prepipe(t_command *command, t_data *data);
int						is_valid_identifier(const char *str);
void					set_exp(t_data *data, char *name, char *value);

//cleaner.c
void					clean_variables(t_vars *vars);
void					clean_envp(char **envp);

#endif
