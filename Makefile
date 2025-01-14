NAME		=		minishell
SRCS		=		$(addprefix $(SRC_DIR)/, main.c signals.c get_prompt.c \
					utils.c init_data.c vars.c tokenizer.c parsing.c exec.c \
					built_in.c)
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