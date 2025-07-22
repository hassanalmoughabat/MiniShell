NAME		:= minishell
LIBFT		:= libft.a
LIBFT_PATH	:= "includes/libft"
PRINTF		:= libftprintf.a
PRINTF_PATH	:= "includes/ft_printf"
CC			:= cc
CFLAGS		:= -Wall  -Wextra -g

BUILTINS	:=	builtins/builtins_utils.c \
				builtins/builtins.c \
				builtins/copy_env.c \
				builtins/env_getters.c \
				builtins/env_list_addition.c \
				builtins/env_utils.c \
				builtins/env.c \
				builtins/error_handle.c \
				builtins/export_utils.c \
				builtins/export.c \
				builtins/extra_functions.c \
				builtins/ft_cd.c \
				builtins/ft_cd_cont.c \
				builtins/ft_echo.c \
				builtins/ft_exit.c \
				builtins/ft_exit_utils.c\
				builtins/key_validators.c \
				builtins/quotes_extra_functions.c \
				builtins/quotes_functions.c \
				builtins/unset.c \
				builtins/value_extractor.c \
				builtins/value_getter.c \
				builtins/value_setter.c

EXPANSION	:=	expansion/expander_extraction.c\
				expansion/expansion_cases.c\
				expansion/expansion_helper.c \
				expansion/expansion.c

SHELL_LVL	:=	shell_lvl/shell_handling.c

SIGNALING	:=	signaling_heredocs/ft_signal.c \
				signaling_heredocs/ft_heredocs.c \
				signaling_heredocs/handle_great_main.c \
				signaling_heredocs/heredoc_helper.c \
				signaling_heredocs/handle_great_utils.c \
				signaling_heredocs/handle_great.c \
				signaling_heredocs/handle_redirection.c \
				signaling_heredocs/handle_input_redirect.c \
				signaling_heredocs/pipes.c  

TOKENIZE	:= 	tokenize/ft_error_handler.c \
				tokenize/ft_list_functions.c \
				tokenize/ft_quotes_handler.c \
				tokenize/ft_quotes_helper.c \
				tokenize/ft_space_handler.c \
				tokenize/ft_special_char_handler.c \
				tokenize/ft_tokenize.c

UTILS		:= 	utils/free_functions.c \
				utils/minihell.c \
				utils/parsing_path_handling.c \
				utils/parsing.c

SRCS		:=	$(BUILTINS)\
				$(EXPANSION)\
				$(SHELL_LVL)\
				$(SIGNALING)\
				$(TOKENIZE)\
				$(UTILS)\
				
OBJS		:= $(SRCS:.c=.o)

READLINE	:= -lreadline

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ -Iinclude -I$(READLINE_PATH)/include

all: $(NAME)

$(LIBFT):
	@make --no-print-directory -C $(LIBFT_PATH)
$(PRINTF):
	@make --no-print-directory -C $(PRINTF_PATH)

$(NAME): $(LIBFT) $(PRINTF) $(OBJS)
	@$(CC) -o $(NAME) $(OBJS) -L$(LIBFT_PATH) -lft -L$(PRINTF_PATH) -lftprintf $(READLINE)

clean:
	@make --no-print-directory clean -C $(LIBFT_PATH)
	@make --no-print-directory clean -C $(PRINTF_PATH)
	@rm -f $(OBJS)

fclean: clean
	@make --no-print-directory fclean -C $(LIBFT_PATH)
	@make --no-print-directory fclean -C $(PRINTF_PATH)
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
