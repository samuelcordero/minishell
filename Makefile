# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/05 13:14:44 by sacorder          #+#    #+#              #
#    Updated: 2023/10/17 11:41:38 by sacorder         ###   ########.fr        #
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
		src/init.c \
		src/redirect_utils.c \
		src/redirect_utils2.c \
		src/builtins.c \
		src/command_tree.c \
		src/executer.c
OBJ = $(SRC:.c=.o)
CC = gcc
UNAME := $(shell uname -s)

ifeq ($(UNAME), Darwin)
	# mac
    LDFLAGS += -L/System/Volumes/Data/sgoinfre/students/sacorder/homebrew/opt/readline/lib -lreadline
	CFLAGS += -I/System/Volumes/Data/sgoinfre/students/sacorder/homebrew/opt/readline/include
else
	#Linux and others...
    LDFLAGS += -lreadline
endif

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

$(LIBFT):
	@make bonus -C ./libft

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@make clean -C ./libft
	$(RM) $(OBJ)
	$(RM) $(OBJBONUS)

fclean:
	@make fclean -C ./libft
	$(RM) $(NAME)
	$(RM) $(OBJ)
	$(RM) $(OBJBONUS)

asan:: CFLAGS += -g3 -fsanitize=address
asan:: LDFLAGS += -g3 -fsanitize=address
asan:: re

re:: fclean 
re:: all

rebonus: fclean bonus

.PHONY: all clean fclean re
