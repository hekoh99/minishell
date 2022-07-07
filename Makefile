NAME = minishell

UTILS = srcs/utils

SRCS		=	main.c \
	$(UTILS)/ft_strdup.c \
	$(UTILS)/ft_strlen.c \

OBJS		= $(SRCS:%.c=%.o)

CC = gcc $(DEBUG)
CFLAGS = -Werror -Wall -Wextra
SAN = -fsanitize=address -g3
DEBUG = -g
#READLINE_LIB 	= -lreadline -L/opt/homebrew/opt/readline/lib
#READLINE_INC	= -I/opt/homebrew/opt/readline/include
READLINE_LIB 	= -lreadline -L${HOME}/.brew/opt/readline/lib
READLINE_INC	= -I${HOME}/.brew/opt/readline/include

READLINE_HAKO_LIB = lreadline -L/opt/homebrew/opt/readline/lib
READLINE_HAKO_INC = -I/opt/homebrew/opt/readline/include

.PHONY		:	all
all			:	$(NAME)

$(NAME)		:	$(OBJS)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(READLINE_LIB) $(READLINE_INC)

$(HAKO)		:	$(OBJS)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(READLINE_HAKO_LIB) $(READLINE_HAKO_INC)

%.o: %.c
	$(CC) $(CFLAGS) $(READLINE_INC) -c $< -o $@

.PHONY		:	clean
clean		:
		rm -f $(OBJS)

.PHONY		:	fclean
fclean		:	clean
		rm -f $(NAME)

.PHONY		:	re
re			:	fclean all
