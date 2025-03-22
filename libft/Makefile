############################################################################################################
# Variables
NAME		=		libft.a

INCLUDE_DIR =		./include
INCLUDE		=		$(addprefix $(INCLUDE_DIR)/, libft.h ft_printf.h get_next_line.h )

VPATH		=		./src ./src/libft ./src/ft_printf ./src/get_next_line
SRCS		=		ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c ft_strcmp.c\
					ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_toupper.c\
					ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c\
					ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c\
					ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c\
					ft_strcat.c ft_putnbr_fd.c ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c\
					ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c\
					ft_isspace.c ft_strcpy.c ft_split_charset.c ft_tracked_alloc.c ft_tracked_alloc_caller.c\
					ft_printf.c ft_printf_chars.c ft_printf_nb.c ft_printf_unb.c ft_printf_hex.c \
					ft_printf_ptr.c ft_printf_utils.c ft_putnbr_base.c\
					get_next_line.c get_next_line_utils.c

# Objects
OBJ_DIR		=		./obj
OBJ			=		$(SRCS:%.c=$(OBJ_DIR)/%.o)

# Compiler
LIB 		=		ar rcs
RM			=		rm -rf
CFLAGS		=		-Wall -Wextra -Werror
CC			=		cc

# Colors
GREEN		=		\033[0;32m
UGREEN		=		\033[4;32m
RED			=		\033[0;31m
YELLOW		=		\033[0;33m
LILA		=		\033[0;35m
NC			=		\033[0m # No color

############################################################################################################
# Rules

all: $(NAME)

$(NAME): $(OBJ) $(INCLUDE)
	@$(LIB) $(NAME) $(OBJ)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@$(RM) $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all bonus clean fclean
############################################################################################################
