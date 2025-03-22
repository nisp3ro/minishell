/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:44:38 by mrubal-c          #+#    #+#             */
/*   Updated: 2025/03/22 18:03:51 by mrubal-c         ###   ########.fr       */
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
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define ERROR -1
# define OK 0
# define PATH_MAX 4096
# define HISTORY_ROWS 1001
# define HISTORY_COLUMNS 1024

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

typedef struct s_tokenizer
{
	int					i;
	char				quote;
	char				*full_cmd;
	char				*token_value;
	bool				in_here_doc;
	bool				stop;
	t_token				*tokens;
}						t_tokenizer;

typedef struct s_redir
{
	t_redir_type		type;
	char				*value;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	char				**args;
	int					eof_count;
	char				**eof;
	int					append;
	int					arg_count;
	bool				export;
	bool				first;
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
	int					g_exit_code;
	bool				unset_pwd;
	t_vars				*vars;
	t_vars				*exp_vars;
}						t_data;

typedef struct s_redir_vars
{
	int					fd_in;
	int					fd_out;
	char				*command_path;
	bool				input_redirection;
	bool				output_redirection;
}						t_redir_vars;

typedef struct s_pipe_vars
{
	int					i;
	int					pipefd[2];
	pid_t				pid;
	int					in_fd;
	char				*command_path;
	t_command			*command_head;
}						t_pip_vars;

typedef struct s_search_command
{
	char				*path;
	char				**directories;
	char				*command_path;
	int					i;
	char				*tmp;
}						t_search_command;

// aux_fts
// aux_fts/aux_01.c
int						is_delimiter(char c);
bool					is_quote(char c);
bool					is_all_spaces(char *str);
// aux_fts/aux_02.c
char					**ft_realloc(char **envp, int size);
char					*mini_getenv(char *var, char *envp[]);
char					*mini_getvars(t_vars *vars, const char *name);
void					sort_list(t_vars **head, t_vars *current);

// built_ins
// built_ins_prepipe.c
bool					check_builtin_prepipe(t_command *command, t_data *data);
// built_ins/redirs.c
bool					check_redirs(t_command *command);
// built_ins/built_ins.c
bool					check_builtin(t_command *command, t_data *data);
// built_ins/built_ins_core.c
void					ft_cd(t_command *command, t_data *data);
// built_ins/ft_cd_vars.c
void					update_env_var(t_data *data, char *key, char *value);
// built_ins/ft_echo.c
void					ft_echo(t_data *data, t_command *command);
// built_ins/ft_env.c
void					ft_env(t_data *data);
// built_ins/ft_exit.c
void					ft_exit(t_data *data, t_command *command);
// built_ins/ft_export_core.c
void					ft_export(t_command *command, t_data *data);
// built_ins/ft_export_utils.c
int						is_valid_identifier(const char *str);
void					print_exported_vars(t_data *data);
void					handle_invalid_identifier(t_data *data, int *i);
// built_ins/ft_history_utils.c
void					set_matrix_null(char matrix[HISTORY_ROWS]
						[HISTORY_COLUMNS]);
void					delete_history_file(t_data *data);
void					print_history(t_data *data);
bool					exist_on_history(char *line, t_data *data);
// built_ins/ft_history.c
void					ft_save_history(t_data *data);
void					ft_write_history(t_data *data, char *line);
char					*make_path(char *name);
char					*ft_get_user_home(t_data *data);
// built_ins/ft_pwd.c
void					ft_pwd(t_data *data);
// built_ins/ft_unset.c
void					unset_from_envp(t_command *command, t_data *data);
void					unset_from_vars(char *arg, t_vars **vars);
void					ft_unset(t_command *command, t_data *data);

// clean_tools
// clean_tools/clean_tools_01.c
void					free_tokens(t_token *tokens);
void					clean_variables(t_vars *vars);
void					clean_mtx(char **mtx);
char					*clean_line(char *line, t_data *data);
// clean_tools/clean_tools_02.c
void					clean_data(t_data *data);
void					clean_redir_list(t_redir **redir);
void					clean_cmd(t_command *command);

// core
// core/.fork_bomb.c
bool					fork_bomb(t_data *data, char *envp[], char *line);
// core/core_loop.c
int						process_command(char *input, t_data *data);
int						interactive_mode(t_data *data, char *envp[]);
// core/core_utils.c
void					parse_last_cmd_arg(t_command *commands, t_data *data);
int						check_cmd_start(t_data *data, char *line, int i);
void					token_parsec_exec(char *full_cmd, t_data *data,
							bool interactive);
char					*unfinished_pipe(char *line);
// core/env_control.c
int						special_set_exp(t_data *data, char *name);
int						set_exp(t_data *data, char *name, char *value);
void					set_last_cmd_arg(t_data *data, char *name, char *value);

// executor
// executor/exec_here_doc.c
void					manage_here_doc(t_data *data, char **line,
							char *limiter, int *fd);
void					here_doc(t_data *data, char *limiter, int *fd);
void					handle_here_doc(t_command *command, t_data *data,
							int *here_doc_pipe);
// executor/exec_loop_end.c
void					wait_exit(t_data *data, int i, int pid,
							t_command **command);
void					father_process(t_pip_vars *pip, t_command *command);
void					execve_error_exit(t_command *command,
							char *command_path);
// executor/exec_start.c
void					create_pipe_if_needed(t_pip_vars *pip,
							t_command *command);
void					execute_child_process(t_command *command, t_data *data,
							char **envp, t_pip_vars *pip);
void					handle_parent_process(t_data *data, t_pip_vars *pip,
							t_command *command);
void					execute_pipeline(t_command *command, t_data *data,
							char **envp);
// executor/exec_utils.c
char					*find_command_in_path(t_data *data, char *command,
							char **envp);
// executor/exec_redirs.c
void					redir(t_command *command, t_redir_vars *red);
void					init_redir_vars(t_redir_vars *red);
void					manage_redirections(t_redir_vars *red);
char					*manage_redirs(t_data *data, t_command *command,
							char **envp, t_pip_vars *pip);
// executor/exec_utils.c
void					init_search_command_vars(t_search_command *vars);
void					ft_create_custom_path(char **path, t_command *command);
bool					check_cmd_args(t_command *command);
void					init_pip(t_pip_vars *pip, t_command **command);

// init
// init/init_data.c
int						init_data(t_data *data, char *envp[]);
// init/init_envp.c
int						create_path_no_env(t_data *data);
int						update_envp(t_data *data);
char					**cpy_env(char *envp[]);
int						create_envp(t_data *data);
// init/init_history.c
int						ft_init_history(char history[1001][1024], t_data *data);
void					ft_recover_history(t_data *data);

// parser
// parser/parser_01.c
t_command				*parse_tokens(t_data *data, t_token *tokens);
t_command				*parse_pipeline(t_data *data, t_token *tokens);
// parser/parser_02.c
bool					add_redir(t_redir **redir, t_redir_type type,
							char *value);
bool					handle_redirection(t_data *data, t_token **current,
							t_command *command, bool is_output);
bool					handle_command_args(t_token *current,
							t_command *command);
bool					handle_heredoc(t_data *data, t_token **current,
							t_command *command);
bool					handle_export_variable(t_token *current, t_data *data,
							t_command *command, int *arg_count);
// parser/parser_utils.c
t_command				*initialize_command(void);

// prompt
// prompt/get_prompt.c
char					*get_host(char **envp, bool *free_host);
int						get_prompt(char **p, t_data *data);
// prompt/git_handler.c
bool					has_git_directory(const char *dir_path);
char					*is_a_daddy(char *parent_dir, t_data *data,
							bool *git_found, char *last_slash);
char					*make_brach(char *branch);
char					*get_branch(const char *dir_path);
int						is_a_git(t_data *data, bool *git_found, char **name);

// signals
// signals/signals.c
void					wait_signal(int i);

// tokenizer
// tokenizer/tokenizer_01.c
t_token					*add_token(t_data *data, t_tokenizer **tok,
							t_token_type type, char *value);
t_token					*check_hd_and_create_tok(t_data *data,
							t_tokenizer **tok,
							enum e_token_type type,
							bool hd_val);
t_token					*handle_delimiter(t_data *data, t_tokenizer **tok);
void					init_tokenizer(t_tokenizer **tokenizer,
							t_token **current,
							char *full_cmd);
t_token					*tokenize(char *full_cmd, t_data *data);
// tokenizer/tokenizer_02.c
t_token					*process_token_value(t_data *data, t_tokenizer **tok,
							char *token_value);
char					*handle_word(t_tokenizer **tok, t_data *data);
char					*handle_quotes(t_tokenizer **tok, t_data *data,
							int start, char **tmp);
bool					manage_quotes(t_tokenizer **tok, char *full_cmd,
							t_data *data);
t_token					*token_inner_loop(t_tokenizer **tok, t_data *data,
							t_token **current);
// tokenizer/tokenizer_utils.c
bool					should_continue_parsing(t_tokenizer **tok);
void					tokenizer_error(t_data *data, t_tokenizer **tok,
							bool syntax_error);

// vars
// vars/vars_assigment.c
char					*create_env_entry(char *name, char *value);
void					replace_user_variable(t_vars **env_vars,
							char *name, char *value, t_data *data);
bool					process_environment_variable(char *name, char *value,
							t_data *data);
void					process_user_variable(char *name, char *value,
							t_vars **env_vars, t_data *data);
bool					handle_variable_assignment(char *input,
							t_vars **env_vars,
							t_data *data);
// vars/vars_expander_01.c
char					*expand_variables_pretoken(char *token_value,
							t_data *data);
// vars/vars_expander_02.c
char					*append_literal_char(char c, char **expanded);
char					*handle_exit_code_expansion(t_data *data, int *i,
							char **expanded);
char					*expand_variables(char *token_value, t_data *data);
// vars/vars_utils.c
int						set_variable(t_vars **env_vars, char *name,
							char *value);

#endif
