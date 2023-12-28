# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/05 13:14:44 by sacorder          #+#    #+#              #
#    Updated: 2023/12/28 10:57:51 by sacorder         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBFT = libft/libft.a
LDFLAGS = -L libft -lft
NAME = minishell
CFLAGS = -Wall -Wextra -Werror -O3 -Iinc -Ilibft
SRC =	src/main.c \
		src/enviroment/enviroment.c \
		src/enviroment/enviroment_extra.c \
		src/enviroment/expander.c \
		src/execute/execute.c \
		src/execute/execute_utils.c \
		src/execute/redirect_utils.c \
		src/execute/path_finder.c \
		src/lexer/lexer.c \
		src/lexer/lexer_utils.c \
		src/heredoc/heredoc.c \
		src/history/history.c \
		src/utils/utils.c \
		src/utils/utils_2.c \
		src/utils/init.c \
		src/utils/syntax_checker.c \
		src/builtins/builtins_utils.c \
		src/builtins/cd.c \
		src/builtins/echo.c \
		src/builtins/exit.c \
		src/builtins/export.c \
		src/builtins/pwd.c \
		src/builtins/unset.c \
		src/bonus/command_tree.c \
		src/bonus/command_tree_utils.c \
		src/bonus/wildcards.c
OBJ = $(SRC:.c=.o)
CC = gcc
UNAME := $(shell uname -s)

ifeq ($(UNAME), Darwin)
	# mac, you may need to change flags in order to compile inside the clusters (checkwhere your readline brew installation is)
    LDFLAGS += -L/usr/local/opt/readline/lib
    LDFLAGS += -L/System/Volumes/Data/sgoinfre/students/${USER}/homebrew/opt/readline/lib -lreadline
    LDFLAGS += -L/opt/homebrew/opt/readline/lib	# macbook guortun
    CFLAGS += -I/opt/homebrew/opt/readline/include # macbook guortun
    CFLAGS += -I/usr/local/opt/readline/include
    CFLAGS += -I/System/Volumes/Data/sgoinfre/students/${USER}/homebrew/opt/readline/include
else
	#Linux and others...
    LDFLAGS += -lreadline
endif

.PHONY: all clean fclean asan debug re

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

$(LIBFT):
	@echo "Compiling libft bonus..."
	@make bonus -s -C ./libft
	@echo "Libft done!"

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@make clean -s -C ./libft
	@echo "Make clean libft..."
	$(RM) $(OBJ)

fclean:
	@make fclean -s -C ./libft
	@echo "Make fclean libft..."
	$(RM) $(NAME)
	$(RM) $(OBJ)

asan:: CFLAGS += -g3 -fsanitize=address
asan:: LDFLAGS += -g3 -fsanitize=address
asan:: re

dbg:: CFLAGS += -g3
dbg:: LDFLAGS += -g3
dbg:: re

re:: fclean 
re:: all
