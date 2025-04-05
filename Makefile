NAME       = minishell

SRCS       = env_utils.c env.c error_handle.c minihell.c builtins.c parsing.c ft_cd.c leak.c export.c builtins_utils.c env_getters.c env_list_addition.c  expansion_helper.c expansion.c export_utils.c extra_functions.c ft_echo.c key_validators.c quotes_extra_functions.c quotes_functions.c unset.c value_extractor.c value_setter_getter.c copy_env.c
OBJS       = $(SRCS:.c=.o)

CC         = cc
CFLAGS     = -Wall -Wextra -g -Iincludes -I$(LIBFT) -I$(PRINTF) -I$(TOKENIZE)

LIBFT      = includes/libft
PRINTF     = includes/ft_printf
TOKENIZE   = tokenize
LIBS       = $(TOKENIZE)/tokenize.a $(LIBFT)/libft.a $(PRINTF)/libftprintf.a
READLINE   = -lreadline

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)/libft.a $(PRINTF)/libftprintf.a $(TOKENIZE)/tokenize.a
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(READLINE) -o $(NAME)

$(LIBFT)/libft.a:
	make -C $(LIBFT)

$(PRINTF)/libftprintf.a:
	make -C $(PRINTF)

$(TOKENIZE)/tokenize.a:
	make -C $(TOKENIZE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT) clean --silent
	@$(MAKE) -C $(PRINTF) clean --silent
	@$(MAKE) -C $(TOKENIZE) clean --silent

fclean: clean
	rm -f $(NAME)

re: fclean all
