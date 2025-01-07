NAME       = minishell.a

SRCS       = env_utils.c env.c error_handle.c minihell.c
OBJS       = ${SRCS:%.c=%.o}

CC         = gcc
CFLAGS     = -Wall -Wextra -Werror -Iincludes
LIBS       = includes/libft/libft.a includes/ft_printf/libftprintf.a
HEADER     = includes/minihell.h

# ANSI Color Codes
RESET      = \033[0m
BOLD       = \033[1m
RED        = \033[31m
YELLOW     = \033[33m
CYAN       = \033[36m

all: echo $(NAME)

echo:
	@echo ""
	@echo "$(RED)######################################################$(RESET)"
	@echo "$(YELLOW)#$(RESET)$(CYAN)               Minishell is making         	    $(YELLOW)#$(RESET)"
	@echo "$(YELLOW)#$(RESET)$(CYAN)       A small copy of Hell awaits you!   	    $(YELLOW)#$(RESET)"
	@echo "$(YELLOW)#$(RESET)$(CYAN)        	Please Be Kind to It		    $(YELLOW)#$(RESET)"
	@echo "$(RED)######################################################$(RESET)"
	@echo ""

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)
	ranlib $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@ -g

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
