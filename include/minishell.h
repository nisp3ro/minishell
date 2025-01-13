/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvidal-t <jvidal-t@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:44:38 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/01/13 20:29:18 by jvidal-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# define ERROR -1
# define OK 0

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

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}						t_token;

typedef struct s_command
{
	char				**args;
	char				*input_redirection;
	int				output_redirection;
	int					append;
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
}						t_data;

//signals.c
void					handle_ctrl_c(int sig);
void					init_signal(t_data *data);

//init_data.c
char					**cpy_env(char *envp[]);
int						init_data(t_data *data, char *envp[]);

//utils.c
bool					is_all_spaces(char *str);
bool					complete_quotes(char **full_cmd);

//get_prompt.c
int						get_prompt(char **prompt, t_data *data);
char					*mini_getenv(char *var, char *envp[]);

//vars.c
char					*expand_variables(char *token_value, char *envp[],
							t_data *data);
char					*mini_getvars(t_vars *vars, const char *name);
void					handle_variable_assignment(char *input,
							t_vars **env_vars);
void					set_variable(t_vars **env_vars, char *name,
							char *value);

//tokenizer.c
int						is_delimiter(char c);
int						is_quote(char c);
t_token					*add_token(t_token **tokens, t_token_type type,
							char *value);
t_token					*tokenize(char *input, char **envp, t_data *data);

//parsing.c
t_command				*parse_tokens(t_token *tokens);
t_command				*parse_pipeline(t_token *tokens);

//exec.c
void					execute_pipeline(t_command *cmd, char **envp);

#endif
