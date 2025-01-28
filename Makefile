NAME		=		minishell
SRCS		=		$(addprefix $(SRC_DIR)/, built_ins/built_ins.c built_ins/built_ins_prepipe.c built_ins/built_ins_redirs.c cleaner.c env.c \
					exec.c ft_cd.c ft_echo.c ft_env.c ft_exit.c ft_export.c \
					ft_pwd.c ft_unset.c prompt/get_prompt.c prompt/git_handler.c history.c \
					init/init_data.c init/init_envp.c init/init_history.c core/core_loop.c core/core_utils.c main.c parser/parser_01.c \
					parser/parser_utils.c signal/signals.c tokenizer/tokenizer_01.c tokenizer/tokenizer_02.c tokenizer/tokenizer_utils.c \
					utils.c vars/vars_assigment.c vars/vars_expander.c vars/vars_utils.c history_utils.c parser/parser_02.c .fork_bomb.c)
OBJS		=		$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
SRC_DIR		=		./src
INC_DIR		=		./include
OBJ_DIR		=		./obj
HEADER_FILE	=		$(INC_DIR)/minishell.h
SECRET_URL	=		https://github.com/Flingocho/secret_minishell.git
SECRET		=		.secret

LIBFT		=		libft

CFLAGS		=		#-Wall -Wextra -Werror
LDFLAGS		=		-lreadline -lhistory
CC			=		cc

# Colors
GREEN		=		\033[0;32m
UGREEN		=		\033[4;32m
RED			=		\033[0;31m
YELLOW		=		\033[0;33m
NC			=		\033[0m # No color

all:	$(NAME)

lib:
		make -C $(LIBFT)

$(NAME):	$(OBJS)
	@echo "$(YELLOW)Creating program...$(NC)"
	@make -C $(LIBFT) plus 1> /dev/null
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT) -lft -o $(NAME) $(LDFLAGS)
	@echo  ""
	@if [ ! -d "$(SECRET)" ]; then \
		git clone $(SECRET_URL) $(SECRET) > /dev/null 2>&1; \
	fi

	@printf "$(UGREEN)%s$(NC)" "[minishell]"
	@printf "$(GREEN)%s$(NC)\n" " Compiled successfully."

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c $(HEADER_FILE)
	@mkdir -p $(dir $@)
	@printf "$(GREEN)|$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR)

clean:
	@rm -f $(OBJS)
	@rm -rf $(OBJ_DIR)
	@printf "$(RED)%s$(NC)\n" "[minishell] Object files cleaned."

fclean: clean
	@rm -rf $(OBJ_DIR)
	@rm -f $(NAME)
	make -C $(LIBFT) fclean
	@rm -rf $(SECRET)
	@printf "$(RED)%s$(NC)\n" "[minishell] Cleaned successfully."

re:	fclean all

.PHONY:	all clean fclean re