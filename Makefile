NAME       = tokenization  # Executable name

SRCS       = tokenize/ft_error_handler.c tokenize/ft_list_functions.c \
             tokenize/ft_quotes_handler.c tokenize/ft_quotes_helper.c \
             tokenize/ft_space_handler.c tokenize/ft_special_char_handler.c \
             tokenize/ft_tokenize.c tokenize/main.c
OBJS       = ${SRCS:.c=.o}

CC         = gcc
CFLAGS     = -Wall -Wextra -Werror -Iincludes
LIBS       = ft_libft/libft.a

all: $(NAME)

# Create the executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

# Rule to create object files from source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
