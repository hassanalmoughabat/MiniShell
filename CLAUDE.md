# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

- **Build**: `make` or `make all` - Compiles the minishell executable
- **Clean**: `make clean` - Removes object files
- **Full Clean**: `make fclean` - Removes object files and executable
- **Rebuild**: `make re` - Full clean and rebuild
- **Run**: `./minishell` - Execute the built minishell

## Project Architecture

This is a **Minishell** implementation - a simplified shell that mimics basic bash functionality. The codebase is organized into modular directories:

### Core Components

- **Main Entry**: `utils/minihell.c` - Contains main loop and shell initialization
- **Tokenization**: `tokenize/` - Lexical analysis of input commands into tokens
- **Expansion**: `expansion/` - Variable expansion (environment variables, $PATH, etc.)
- **Built-ins**: `builtins/` - Implementation of shell built-in commands (cd, echo, env, export, pwd, unset, exit)
- **Pipes & Redirection**: `pipes/` and `signaling_heredocs/` - Handle command piping, I/O redirection, and heredocs
- **Signal Handling**: Global signal management for proper shell behavior
- **Environment**: Environment variable management and shell level tracking

### Key Data Structures

- `t_shell`: Main shell state containing environment, tokens, and current command
- `t_token`: Doubly-linked list representing parsed command tokens
- `t_env`: Environment variables as linked list
- `t_signal`: Global signal handling state

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