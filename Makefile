NAME       = minishell

SRCS       = env_utils.c env.c error_handle.c minihell.c
OBJS       = $(SRCS:.c=.o)

CC         = gcc
CFLAGS     = -Wall -Wextra -Werror -Iincludes

LIBFT      = includes/libft/libft.a
PRINTF     = includes/ft_printf/libftprintf.a
TOKENIZE   = tokenize/tokenization.a
LIBS       = $(TOKENIZE) $(LIBFT) $(PRINTF)
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

$(LIBFT):
	make -C includes/libft

$(PRINTF):
	make -C includes/ft_printf

$(TOKENIZE):
	make -C tokenize
