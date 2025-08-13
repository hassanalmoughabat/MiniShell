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
				builtins/export_utils1.c \
				builtins/export_utils2.c \
				builtins/export.c \
				builtins/extra_functions.c \
				builtins/ft_cd.c \
				builtins/ft_cd_utils1.c \
				builtins/ft_cd_utils2.c \
				builtins/ft_cd_utils3.c \
				builtins/ft_echo.c \
				builtins/ft_echo_utils1.c \
				builtins/ft_echo_utils2.c \
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
				expansion/expansion_utils.c\
				expansion/expansion_utils2.c\
				expansion/expansion_utils3.c\
				expansion/expansion_cases.c\
				expansion/expansion_helper.c \
				expansion/expansion.c

SHELL_LVL	:=	shell_lvl/shell_handling.c

SIGNALING	:=	signaling_heredocs/ft_signal.c \
				signaling_heredocs/ft_signals1.c \
				signaling_heredocs/external_cmd.c \
				signaling_heredocs/error_msg.c \
				signaling_heredocs/external_cmd_utils.c \
				signaling_heredocs/ft_heredoc1.c \
				signaling_heredocs/ft_heredoc_helper.c \
				signaling_heredocs/ft_heredoc2.c \
				signaling_heredocs/ft_heredoc3.c \
				signaling_heredocs/ft_heredoc4.c \
				signaling_heredocs/ft_heredoc5.c \
				signaling_heredocs/ft_heredoc6.c \
				signaling_heredocs/ft_heredoc7.c \
				signaling_heredocs/ft_heredoc8.c \
				signaling_heredocs/ft_heredoc9.c \
				signaling_heredocs/handle_redirection1.c \
				signaling_heredocs/handle_redirection2.c \
				signaling_heredocs/handle_redirection3.c \
				signaling_heredocs/handle_input_redirect.c \
				signaling_heredocs/handle_input_redirect1.c

PIPES		:=	pipes/pipes_utils.c \
				pipes/pipes_heredoc.c \
				pipes/pipes_segment.c \
				pipes/pipes_segment_utils.c \
				pipes/pipes_segment_utils2.c \
				pipes/pipes_redirect.c \
				pipes/pipes_main.c \
				pipes/pipes_heredoc_utils.c \
				pipes/pipes_heredoc_utils2.c \
				pipes/pipes_heredoc_util2.c \
				pipes/pipes_utils2.c \
				pipes/pipes_utils3.c \
				pipes/pipes_utils4.c \
				pipes/pipes_utils5.c \
				pipes/pipes_utils6.c


TOKENIZE	:= 	tokenize/ft_error_handler.c \
				tokenize/ft_list_functions.c \
				tokenize/ft_quotes_handler.c \
				tokenize/ft_quotes_helper.c \
				tokenize/ft_space_handler.c \
				tokenize/ft_special_char_handler.c \
				tokenize/ft_tokenize.c

UTILS		:= 	utils/free_functions.c \
				utils/init.c \
				utils/minihell.c \
				utils/parsing1.c \
				utils/parsing2.c \
				utils/parsing3.c \
				utils/path_finder.c

SRCS		:=	$(BUILTINS)\
				$(EXPANSION)\
				$(SHELL_LVL)\
				$(SIGNALING)\
				$(PIPES)\
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
