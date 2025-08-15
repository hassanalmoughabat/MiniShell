# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

- **Build**: `make` or `make all` - Compiles the minishell executable
- **Clean**: `make clean` - Removes object files
- **Full Clean**: `make fclean` - Removes object files and executable
- **Rebuild**: `make re` - Full clean and rebuild
- **Run**: `./minishell` - Execute the built minishell
- **Debug Build**: `make CFLAGS="-Wall -Wextra -g -fsanitize=address"` - Build with AddressSanitizer for debugging
- **Memory Check**: `valgrind --leak-check=full ./minishell` - Check for memory leaks

## Project Architecture

This is a **Minishell** implementation - a simplified shell that mimics basic bash functionality. The codebase is organized into modular directories:

### Core Components

- **Main Entry**: `utils/minihell.c` - Contains main loop and shell initialization
- **Tokenization**: `tokenize/` - Lexical analysis of input commands into tokens
- **Expansion**: `expansion/` - Variable expansion (environment variables, $PATH, etc.)
- **Built-ins**: `builtins/` - Implementation of shell built-in commands (cd, echo, env, export, pwd, unset, exit)
- **Pipes & Redirctione**: `pipes/` and `signaling_heredocs/` - Handle command piping, I/O redirection, and heredocs
- **Signal Handling**: Global signal management for proper shell behavior
- **Environment**: Environment variable management and shell level tracking

### Key Data Structures

- `t_shell`: Main shell state containing environment, tokens, and current command
- `t_token`: Doubly-linked list representing parsed command tokens (defined in `tokenize/token.h`)
- `t_env`: Environment variables as linked list with exit status tracking
- `t_signal`: Global signal handling state (`g_signal` global variable)
- `t_gc`: Garbage collector for memory management - use `ft_malloc()` for tracked allocation
- `t_pip`: Pipeline data structure for command piping
- `t_heredoc_info`: Heredoc file descriptor and position tracking

### Dependencies

- **libft**: Custom C standard library implementation in `includes/libft/`
- **ft_printf**: Custom printf implementation in `includes/ft_printf/`
- **readline**: GNU readline library for input handling

### Architecture Flow

1. Input reading with readline
2. Tokenization into command tokens
3. Variable expansion
4. Command execution (built-ins vs external commands)
5. Pipe and redirection handling
6. Signal management throughout

The shell handles complex features like heredocs, nested pipes, environment variable expansion, and proper signal handling to closely mimic bash behavior.

## Memory Management

- Use `ft_malloc(&shell->gc, size)` instead of `malloc()` for automatic cleanup
- Call `ft_free_all(&shell->gc)` to free all tracked allocations
- The garbage collector (`t_gc`) automatically tracks allocated memory to prevent leaks

## Important Notes

- Global signal state in `g_signal` manages SIGINT/SIGQUIT behavior
- Shell level tracking via `SHLVL` environment variable (see `shell_lvl/` directory)
- All built-in commands modify the shell's environment state directly
- Token parsing handles quotes, variable expansion, and special characters
- Compile flags include `-g` for debugging - use GDB or valgrind for troubleshooting
- when you want see if there is any leaks use valgrind --leak-check=full -q ./minishell
- to check leak use: valgrind --leak-check=full -q