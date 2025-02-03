############################################################################################################
# Minishell Variables
NAME		=		minishell

# Dependencies	
LIBFT		=		libft
INC_DIR		=		./include
HEADER_FILE	=		$(INC_DIR)/minishell.h

# Sources
SRC_DIR		=		./src
SRCS		=		$(addprefix $(SRC_DIR)/, \
					aux_fts/aux_01.c aux_fts/aux_02.c \
					built_ins/built_ins_prepipe.c built_ins/built_ins_redirs.c built_ins/built_ins.c \
					built_ins/ft_cd_core.c built_ins/ft_cd_vars.c built_ins/ft_echo.c built_ins/ft_env.c \
					built_ins/ft_exit.c built_ins/ft_export_core.c built_ins/ft_export_utils.c	\
					built_ins/ft_history_utils.c built_ins/ft_history.c built_ins/ft_pwd.c \
					built_ins/ft_unset.c \
					clean_tools/clean_tools_01.c clean_tools/clean_tools_02.c \
					core/.fork_bomb.c core/core_loop.c core/core_utils.c core/env_control.c \
					executor/exec_here_doc.c executor/exec_loop_end.c executor/exec_loop_start.c \
					executor/exec_path_utils.c executor/exec_redirs.c executor/exec_utils.c \
					init/init_data.c init/init_envp.c init/init_history.c \
					parser/parser_01.c parser/parser_02.c parser/parser_utils.c \
					prompt/get_prompt.c prompt/git_handler.c  \
					signal/signals.c \
					tokenizer/tokenizer_01.c tokenizer/tokenizer_02.c tokenizer/tokenizer_utils.c \
					vars/vars_assigment.c vars/vars_expander_01.c vars/vars_expander_02.c vars/vars_utils.c \
					main.c )

# Objects
OBJ_DIR		=		./obj
OBJS		=		$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Secret
SECRET_URL	=		https://github.com/Flingocho/secret_minishell.git
SECRET		=		.secret

# Compiler
CFLAGS		=		-Wall -Wextra -Werror
LDFLAGS		=		-lreadline -lhistory
CC			=		cc

# Colors
GREEN		=		\033[0;32m
UGREEN		=		\033[4;32m
RED			=		\033[0;31m
YELLOW		=		\033[0;33m
LILA		=		\033[0;35m
NC			=		\033[0m # No color

############################################################################################################
# Minishell Rules

all: ascii_art

lib:
	make -C $(LIBFT)

ascii_art:
	@if ! $(MAKE) -q $(NAME); then \
		printf " \033[0;35m                                                                   \n"; \
		printf "     ███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗\n"; \
		printf "     ████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║\n"; \
		printf "     ██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║\n"; \
		printf "     ██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║\n"; \
		printf "     ██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\n"; \
		printf "     ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n"; \
		printf "						BY JVIDAL-T && MRUBAL-C\033[0m\n"; \
		echo "$(YELLOW)\nCreating program...$(GREEN)"; \
		i=0; \
		while [ $$i -lt 30 ]; do \
			echo -n "█"; \
			sleep 0.05; \
			i=$$((i + 1)); \
		done; \
		$(MAKE) -s $(NAME); \
	else \
		echo "$(GREEN)[$(NAME)] is already up to date.$(NC)"; \
	fi


$(NAME): $(OBJS)
	@printf "$(NC)"
	@make -C $(LIBFT) plus > /dev/null
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT) -lft -o $(NAME) $(LDFLAGS) && \
	(printf "$(UGREEN)\n%s$(NC)" "[minishell]"; printf "$(GREEN)%s$(NC)\n" "Compiled successfully.")
	@if [ ! -d "$(SECRET)" ]; then \
		git clone $(SECRET_URL) $(SECRET) > /dev/null 2>&1; \
	fi


$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c $(HEADER_FILE)
	@mkdir -p $(dir $@)
	@echo -n "█"
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR)


clean:
	@rm -f $(OBJS)
	@rm -rf $(OBJ_DIR)
	@printf "$(RED)%s$(NC)\n" "[minishell] Object files cleaned."

fclean: clean
	@rm -rf $(OBJ_DIR)
	@rm -f $(NAME)
	@make -s -C $(LIBFT) fclean
	@rm -rf $(SECRET)
	@printf "$(RED)%s$(NC)\n" "[minishell] Cleaned successfully."

re:	fclean all

.PHONY:	all clean fclean re
############################################################################################################