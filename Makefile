NAME = minishell
HAKO = hako

HAKO = hako
YUB = yub

UTILS = srcs/utils
BUILTIN = srcs/builtin
EXECUTE = srcs/execute

SRCS		= $(UTILS)/ft_strdup.c \
	$(UTILS)/ft_strlen.c \
	$(UTILS)/ft_substr.c \
	$(UTILS)/ft_isalphadigit.c \
	$(UTILS)/ft_system_call.c \
	$(UTILS)/ft_system_call2.c \
	$(UTILS)/builtin_utils.c \
	$(UTILS)/ft_strchr.c \
	$(UTILS)/ft_strncmp.c \
	$(UTILS)/token_free.c \
	$(UTILS)/ft_strjoin.c \
	$(UTILS)/ft_strlcpy.c \
	$(UTILS)/ft_itoa.c \
	$(UTILS)/ft_atoi.c \
	$(UTILS)/ft_split.c \
	$(UTILS)/free_matrix.c \
	$(UTILS)/error.c \
	$(UTILS)/gnl.c \
	$(BUILTIN)/cd.c \
	$(BUILTIN)/echo.c \
	$(BUILTIN)/env.c \
	$(BUILTIN)/exit.c \
	$(BUILTIN)/export.c \
	$(BUILTIN)/export_util.c \
	$(BUILTIN)/export_util2.c \
	$(BUILTIN)/pwd.c \
	$(BUILTIN)/unset.c \
	$(EXECUTE)/execute.c \
	$(EXECUTE)/execute_util.c \
	$(EXECUTE)/execve.c \
	$(EXECUTE)/execve_util.c \
	srcs/token.c \
	srcs/signal.c \
	srcs/heredoc_tmp_files.c \
	srcs/parse_error.c \
	srcs/trim.c \
	srcs/split_by_sep.c \
	srcs/parse_check.c \

OBJS		= $(SRCS:%.c=%.o)

CC = cc $(DEBUG)
# CFLAGS = -Werror -Wall -Wextra
SAN = -fsanitize=address -g3
DEBUG = -g

# todo: use this
BREW_DIR		:= $(shell brew --prefix readline)
READLINE_LIB	:= $(addprefix $(BREW_DIR)/, lib)
READLINE_INC	:= -I $(addprefix $(BREW_DIR)/, include)
READLINE_LIB	:= -lreadline -L$(READLINE_LIB)


# 맥북환경
# READLINE_LIB 	= -lreadline -L/opt/homebrew/opt/readline/lib
# READLINE_INC	= -I/opt/homebrew/opt/readline/include
# 클러스터환경
# READLINE_LIB 	= -lreadline -L${HOME}/.brew/opt/readline/lib
# READLINE_INC	= -I${HOME}/.brew/opt/readline/include

READLINE_HAKO_LIB = -lreadline -L/opt/homebrew/opt/readline/lib
READLINE_HAKO_INC = -I/opt/homebrew/opt/readline/include

.PHONY	:all
all		:$(NAME)

$(NAME)	:$(OBJS)
	$(CC) -o $(NAME) main.c $(OBJS) $(READLINE_LIB) $(READLINE_INC)

$(HAKO)	:$(OBJS)
	$(CC) -fsanitize=address -g -o $(NAME) main.c $(OBJS) $(READLINE_HAKO_LIB) $(READLINE_HAKO_INC)

%.o: %.c
	$(CC) $(CFLAGS) $(READLINE_INC) -c $< -o $@

.PHONY	:clean
clean	:
	rm -f $(OBJS)

.PHONY	:fclean
fclean	:clean
	rm -f $(NAME)

.PHONY	:re
re		:$(MAKE) fclean
	$(MAKE) all
