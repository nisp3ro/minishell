NAME		=		minishell
SRCS		=		$(addprefix $(SRC_DIR)/, main.c)
OBJS		=		$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
SRC_DIR		=		./src
INC_DIR		=		./include
OBJ_DIR		=		./$(SRC_DIR)/obj
HEADER_FILE	=		$(INC_DIR)/minishell.h

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

$(NAME):	$(OBJS)
	@echo "$(YELLOW)Creating program...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)
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
	@printf "$(RED)%s$(NC)\n" "[minishell] Cleaned successfully."

re:	fclean all

.PHONY:	all clean fclean re