NAME       = minishell

SRCS       = env_utils.c env.c error_handle.c minihell.c
OBJS       = ${SRCS:.c=.o}

CC         = gcc
CFLAGS     = -Wall -Wextra -Werror -Iincludes
LIBS       = tokenize/tokenization.a includes/libft/libft.a includes/ft_printf/libftprintf.a
READLINE   = -lreadline

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(READLINE) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
