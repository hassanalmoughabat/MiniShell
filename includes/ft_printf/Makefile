.SILENT:
SRCS=    ft_printf.c ft_putchar.c ft_putstr.c ft_putnbr.c ft_putunbr.c \
            ft_putaddr.c ft_puthex.c

OBJS	= ${SRCS:%.c=%.o}

FLAGS	= -Wall -Wextra -Werror

all: $(NAME) 

$(NAME): $(OBJS)
	cc $(FLAGS) -c $(SRCS) -I ./
	ar rc $(NAME) $(OBJS)

%.o: %.c $(HEADER)
	@$(CC) -c $(CFLAGS) $<

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all