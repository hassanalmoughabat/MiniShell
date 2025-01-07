NAME       = minishell  # Executable name, not a library

SRCS       = env_utils.c env.c error_handle.c minihell.c
OBJS       = ${SRCS:%.c=%.o}

CC			= gcc
CFLAGS		= -Wall -Wextra -Werror  -Iincludes 
LIBS		= includes/libft/libft.a includes/ft_printf/libftprintf.a
HEADER		= includes/minihell.h
READLINE	= -lreadline

all: $(NAME)

# Create the executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(READLINE) -o $(NAME)  # Link object files to create the executable

# Rule to create object files from source files
%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)  # Remove the executable

re: fclean all
