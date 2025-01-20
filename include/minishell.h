/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvidal-t <jvidal-t@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:44:38 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/20 17:40:26 by jvidal-t         ###   ########.fr       */
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
# define HISTORY_ROWS 1001
# define HISTORY_COLUMNS 1024

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
	int					fd;
	char				history[HISTORY_ROWS][HISTORY_COLUMNS];
	int					hist_size;
	t_vars				*vars;
	t_vars				*exp_vars;
}						t_data;

// init_data.c
int						ft_init_history(char history[1001][1024], t_data *data);
void					ft_recover_history(t_data *data);
int						create_exp_vars(t_data *data, t_vars **exp_v, int flag);
int						handle_no_env(t_data *data);
int						init_data(t_data *data, char *envp[]);

// env.c
int						set_exp(t_data *data, char *name, char *value);
void					set_last_cmd_arg(t_data *data, char *name, char *value);
int						update_envp(t_data *data);
char					**cpy_env(char *envp[]);
int						create_envp(t_data *data);

// history.c
void					ft_save_history(t_data *data);
void					ft_write_history(t_data *data, char *line);
char					*make_path(char *name);
char					*ft_get_user_home(t_data *data);

// signals.c
void					wait_signal(int i);

// loop.c
int						interactive_mode(t_data *data, char *envp[]);

// get_prompt.c
char					*get_host(char **envp, int *free_host);
int						get_prompt(char **p, t_data *data);

// git_handler.c
bool					has_git_directory(const char *dir_path);
char					*is_a_daddy(char *parent_dir, t_data *data,
							bool *git_found, char *last_slash);
char					*get_branch(const char *dir_path);
int						is_a_git(t_data *data, bool *git_found, char **name);

// tokenize.c
t_token					*add_token(t_token **tokens, t_token_type type,
							char *value);
t_token					*tokenize(char *line, t_data *data);

// parser.c
t_redir					*add_redir(t_redir **redir, t_redir_type type,
							char *value);
t_command				*parse_tokens(t_data *data, t_token *tokens);
t_command				*parse_pipeline(t_data *data, t_token *tokens);

// vars.c
void					handle_variable_assignment(char *input,
							t_vars **env_vars, t_data *data);
int						set_variable(t_vars **env_vars, char *name,
							char *value);
char					*expand_variables(char *token_value, char *envp[],
							t_data *data);

// builtin.c
bool					check_builtin(t_command *command, t_data *data);
bool					check_redirs(t_command *command);
bool					check_builtin_prepipe(t_command *command, t_data *data);

// BULTINS
// ft_cd.c
void					ft_cd(t_command *command, t_data *data);
// ft_echo.c
void					ft_echo(t_command *command);
// ft_env.c
void					ft_env(t_data *data);
// ft_exit.c
void					ft_exit(t_data *data, t_command *command);
// ft_export.c
int						is_valid_identifier(const char *str);
void					ft_export(t_command *command, t_data *data);
// ft_pwd.c
void					ft_pwd(t_data *data);
// ft_unset.c
void					unset_from_envp(t_command *command, t_data *data);
void					unset_from_vars(t_command *command, t_vars **vars);
void					ft_unset(t_command *command, t_data *data);

// execute.c
void					here_doc(t_data *data, char *limiter);
void					execute_pipeline(t_command *command, t_data *data,
							char **envp);

// cleaner.c
void					free_tokens(t_token *tokens);
void					clean_variables(t_vars *vars);
void					clean_mtx(char **mtx);

// utils.c
char					**ft_realloc(char **envp, int size);
int						is_delimiter(char c);
bool					is_quote(char c);
char					*mini_getenv(char *var, char *envp[]);
char					*mini_getvars(t_vars *vars, const char *name);
bool					is_all_spaces(char *str);
bool					complete_quotes(char **full_cmd);
void					sort_list(t_vars **head, t_vars *current);
// history_utils.c
void					print_history(t_data *data);
void					delete_history_file(t_data *data);
bool					exist_on_history(char *line,
							char history[HISTORY_ROWS][HISTORY_COLUMNS]);

#endif
