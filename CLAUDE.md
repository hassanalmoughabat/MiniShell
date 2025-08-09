# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Coding Standards

This project follows **42 School standards**:
- **Norminette compliance** - All code must pass norminette checks
- **Standard C (C99)** - No C++ features, GNU extensions, or non-standard libraries
- Function length limit (25 lines), variable declarations at top of functions
- Specific naming conventions (snake_case for functions, t_ prefix for types)

## Build Commands

- `make` or `make all` - Build the minishell executable
- `make clean` - Remove object files
- `make fclean` - Remove object files and executable
- `make re` - Clean rebuild (fclean + all)
- `norminette` - Check code compliance with 42 School standards (run on all .c and .h files)

The build system automatically compiles dependencies (libft and ft_printf) before building the main executable. Compilation uses `cc` with flags `-Wall -Wextra -g` and links against readline library.

## Architecture Overview

This is a minishell implementation (bash-like shell) written in C with modular architecture:

### Core Data Structures

- `t_shell` - Main shell state containing environment, current command, tokenized input, and shell level tracking
- `t_token` - Doubly-linked list representing parsed command tokens with types (T_IDENTIFIER, T_PIPE, T_LESS, T_GREAT, T_DLESS, T_DGREAT)
- `t_env` - Environment variable linked list with key-value pairs and exit status
- `t_pipe_data` - Manages piping execution with file descriptors and heredoc information
- `t_heredoc_info` - Tracks heredoc file descriptors and cleanup for proper resource management
- `t_signal` - Global signal handling structure for SIGINT/SIGQUIT management

### Module Organization

**tokenize/** - Lexical analysis and parsing
- Converts input strings into token linked lists
- Handles quotes, special characters, and operators
- Core files: `ft_tokenize.c`, `ft_quotes_handler.c`, `ft_special_char_handler.c`

**builtins/** - Built-in shell commands
- Implements: `cd`, `echo`, `env`, `export`, `pwd`, `unset`, `exit`
- Environment variable management and validation
- Key files: `ft_cd.c`, `ft_echo.c`, `export.c`, `env.c`

**expansion/** - Variable expansion and substitution
- Handles `$VAR` expansion including special variables like `$?`
- Quote handling during expansion
- Files: `expansion.c`, `expansion_cases.c`

**pipes/** - Command execution and I/O redirection
- Multi-command pipeline execution
- Heredoc processing (`<<`)
- Input/output redirection (`<`, `>`, `>>`)
- Files: `pipes_main.c`, `pipes_heredoc.c`, `pipes_redirect.c`

**signaling_heredocs/** - Signal handling and heredoc management
- SIGINT/SIGQUIT handling during heredoc input and command execution
- Heredoc input processing with expansion and quote handling
- External command execution and error handling
- Files: `ft_signal.c`, `ft_heredocs.c`, `external_cmd.c`, `handle_redirection*.c`

**utils/** - Core utilities and memory management
- Main program entry point and interactive shell loop (`minihell.c`)
- Parsing coordination and path resolution
- Shell initialization and memory cleanup functions
- Files: `parsing.c`, `free_functions.c`, `init.c`, `path_finder.c`

**shell_lvl/** - Shell level management
- Handles SHLVL environment variable for nested shell instances

### Key Dependencies

- **readline library** - Command line input with history
- **libft** - Custom C library (includes/libft/)
- **ft_printf** - Custom printf implementation (includes/ft_printf/)

### Execution Flow

1. **Input** → **tokenize/** (lexing/parsing)
2. **Tokens** → **expansion/** (variable substitution)
3. **Expanded tokens** → **pipes/** or **builtins/** (execution)
4. **signaling_heredocs/** handles signals throughout

### Important Implementation Details

- **Global Signal Management**: Uses `g_signal` structure to handle SIGINT/SIGQUIT across the entire program
- **Environment Handling**: Environment variables stored as linked list (`t_env`), dynamically converted to array format when needed for `execve`
- **Process Management**: Pipe execution creates child processes with careful file descriptor management and cleanup
- **Heredoc Processing**: Heredocs are pre-processed before command execution, stored in temporary file descriptors with proper expansion handling
- **Memory Management**: Extensive cleanup functions in `utils/free_functions.c` for preventing leaks
- **Quote Handling**: Complex quote processing during tokenization and expansion phases
- **Path Resolution**: Dynamic PATH variable parsing for external command execution

### Testing and Quality

- Run `norminette` on all source files before committing to ensure 42 School compliance
- Test with various shell commands, pipes, redirections, and edge cases
- Pay special attention to signal handling during heredoc input and command execution
- Verify proper cleanup of file descriptors and memory in all execution paths