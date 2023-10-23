# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/05 13:14:44 by sacorder          #+#    #+#              #
#    Updated: 2023/10/23 16:15:53 by sacorder         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBFT = libft/libft.a
LDFLAGS = -L libft -lft
NAME = minishell
CFLAGS = -Wall -Wextra -Werror -O3 -Iinc -Ilibft
SRC =	src/minishell.c \
		src/lexer.c \
		src/lexer_utils.c \
		src/expander.c \
		src/heredoc.c \
		src/init.c \
		src/redirect_utils.c \
		src/builtins.c \
		src/command_tree.c \
		src/execute.c \
		src/enviroment.c
OBJ = $(SRC:.c=.o)
CC = gcc
UNAME := $(shell uname -s)

ifeq ($(UNAME), Darwin)
	# mac, you may need to change flags in order to compile inside the clusters (checkwhere your readline brew installation is)
    LDFLAGS += -L/usr/local/opt/readline/lib
    LDFLAGS += -L/System/Volumes/Data/sgoinfre/students/${USER}/homebrew/opt/readline/lib -lreadline
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
