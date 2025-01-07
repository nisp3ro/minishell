# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrubal-c <mrubal-c@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/20 20:10:56 by mrubal-c          #+#    #+#              #
#    Updated: 2025/01/05 14:18:28 by mrubal-c         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

LIBFT_DIR =		./src/libft
GNL_DIR = 		./src/gnl
PRINTF_DIR = 	./src/printf

OBJ_DIR	=		./ofiles

LIBFT_SRC =		$(LIBFT_DIR)/ft_isalpha.c $(LIBFT_DIR)/ft_isdigit.c $(LIBFT_DIR)/ft_isalnum.c\
				$(LIBFT_DIR)/ft_isascii.c $(LIBFT_DIR)/ft_isprint.c $(LIBFT_DIR)/ft_strlen.c\
				$(LIBFT_DIR)/ft_memset.c $(LIBFT_DIR)/ft_bzero.c $(LIBFT_DIR)/ft_memcpy.c\
				$(LIBFT_DIR)/ft_memmove.c $(LIBFT_DIR)/ft_strlcpy.c $(LIBFT_DIR)/ft_strlcat.c\
				$(LIBFT_DIR)/ft_toupper.c $(LIBFT_DIR)/ft_tolower.c $(LIBFT_DIR)/ft_strchr.c\
				$(LIBFT_DIR)/ft_strrchr.c $(LIBFT_DIR)/ft_strncmp.c $(LIBFT_DIR)/ft_memchr.c\
				$(LIBFT_DIR)/ft_memcmp.c $(LIBFT_DIR)/ft_strnstr.c $(LIBFT_DIR)/ft_atoi.c\
				$(LIBFT_DIR)/ft_calloc.c $(LIBFT_DIR)/ft_strdup.c $(LIBFT_DIR)/ft_substr.c\
				$(LIBFT_DIR)/ft_strjoin.c $(LIBFT_DIR)/ft_strtrim.c $(LIBFT_DIR)/ft_split.c\
				$(LIBFT_DIR)/ft_itoa.c $(LIBFT_DIR)/ft_strmapi.c $(LIBFT_DIR)/ft_striteri.c\
				$(LIBFT_DIR)/ft_putchar_fd.c $(LIBFT_DIR)/ft_putstr_fd.c $(LIBFT_DIR)/ft_strcpy.c\
				$(LIBFT_DIR)/ft_putendl_fd.c $(LIBFT_DIR)/ft_putnbr_fd.c $(LIBFT_DIR)/ft_lstnew.c\
				$(LIBFT_DIR)/ft_lstadd_front.c $(LIBFT_DIR)/ft_lstsize.c $(LIBFT_DIR)/ft_lstlast.c\
				$(LIBFT_DIR)/ft_lstadd_back.c $(LIBFT_DIR)/ft_lstdelone.c $(LIBFT_DIR)/ft_strcat.c\
				$(LIBFT_DIR)/ft_lstclear.c $(LIBFT_DIR)/ft_lstiter.c $(LIBFT_DIR)/ft_lstmap.c\
				$(LIBFT_DIR)/ft_strcmp.c $(LIBFT_DIR)/ft_isspace.c

GNL_SRC = 		$(GNL_DIR)/get_next_line.c $(GNL_DIR)/get_next_line_utils.c

PRINTF_SRC = 	$(PRINTF_DIR)/ft_printf.c $(PRINTF_DIR)/ft_printf_char_n_strings.c\
				$(PRINTF_DIR)/ft_printf_hexa_n_pointers.c $(PRINTF_DIR)/ft_printf_numbers.c

OBJ_LIBFT = 	$(LIBFT_SRC:$(LIBFT_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ_GNL = 		$(GNL_SRC:$(GNL_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ_PRINTF = 	$(PRINTF_SRC:$(PRINTF_DIR)/%.c=$(OBJ_DIR)/%.o)

I_LIB = 		./include/libft.h
I_GNL = 		./include/get_next_line.h
I_PRINT = 		./include/ft_printf.h

LIB = ar rcs
RM = rm -f

CC = cc
CFLAGS = -Wall -Wextra -Werror

# Colors
GREEN = \033[0;32m
UGREEN = \033[4;32m
RED = \033[0;31m
YELLOW = \033[0;33m
NC = \033[0m # No color

# Rules
all: $(NAME)

$(NAME): $(OBJ_LIBFT)
	@echo "$(YELLOW)Creating library...$(NC)"
	$(LIB) $(NAME) $(OBJ_LIBFT)
	@echo "$(GREEN)Library created 👍: $(NAME)$(NC)"

plus: $(OBJ_LIBFT) $(OBJ_GNL) $(OBJ_PRINTF)
	@echo "$(YELLOW)Creating library...$(NC)"
	$(LIB) $(NAME) $(OBJ_LIBFT) $(OBJ_GNL) $(OBJ_PRINTF)
	@echo  ""
	@printf "$(UGREEN)%s$(NC)" "[libft]"
	@printf "$(GREEN)%s$(NC)\n" " Compiled successfully."
	

$(OBJ_DIR)/%.o: $(LIBFT_DIR)/%.c $(I_LIB)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I./include -c $< -o $@
	@printf "$(GREEN)|$(NC)"

$(OBJ_DIR)/%.o: $(GNL_DIR)/%.c $(I_GNL)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I./include -c $< -o $@
	@printf "$(GREEN)|$(NC)"

$(OBJ_DIR)/%.o: $(PRINTF_DIR)/%.c $(I_PRINT)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I./include -c $< -o $@
	@printf "$(GREEN)|$(NC)"

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

replus: fclean plus

.PHONY: all plus clean fclean re replus

