NAME		=		minishell
SRCS		=		$(addprefix $(SRC_DIR)/, built_in.c cleaner.c env.c \
					exec.c ft_cd.c ft_echo.c ft_env.c ft_exit.c ft_export.c \
					ft_pwd.c ft_unset.c get_prompt.c git_handler.c history.c \
					init_data.c loop.c main.c parsing.c signals.c tokenizer.c \
					utils.c vars.c ft_history.c)
OBJS		=		$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
SRC_DIR		=		./src
INC_DIR		=		./include
OBJ_DIR		=		./$(SRC_DIR)/obj
HEADER_FILE	=		$(INC_DIR)/minishell.h

LIBFT		=		libft

CFLAGS		=		#-Wall -Wextra -Werror
LDFLAGS		=		-lreadline
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
	@printf "$(UGREEN)%s$(NC)" "[minishell]"
	@printf "$(GREEN)%s$(NC)\n" " Compiled successfully."

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c $(HEADER_FILE)
	@mkdir -p $(OBJ_DIR)  # Ensure the OBJ_DIR exists
	@printf "$(GREEN)|$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR)

clean:
	@rm -f $(OBJS)
	@printf "$(RED)%s$(NC)\n" "[minishell] Object files cleaned."

fclean: clean
	@rm -rf $(OBJ_DIR)
	@rm -f $(NAME)
	make -C $(LIBFT) fclean
	@printf "$(RED)%s$(NC)\n" "[minishell] Cleaned successfully."

re:	fclean all

.PHONY:	all clean fclean re