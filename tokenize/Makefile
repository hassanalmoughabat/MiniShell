NAME  = tokenize.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRCS = ft_error_handler.c ft_list_functions.c  ft_quotes_handler.c ft_quotes_helper.c  ft_space_handler.c ft_special_char_handler.c  ft_tokenize.c 
OBJS = $(SRCS:.c=.o)
LIBS       = ../includes/libft/libft.a

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(LIBS) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) 

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re 
