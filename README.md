# 🐚 Minishell

[![42 Project](https://img.shields.io/badge/42-Project-blue)](https://42.fr/)
[![Language](https://img.shields.io/badge/language-C-orange.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

A tiny Unix shell written in C as part of the [42 curriculum](https://42.fr/).  
The goal of **Minishell** is to understand how shells work by re-creating a simplified version of `bash`.

---

## ✨ Features

- **Prompt display** that waits for user input
- **Command execution**:
  - Search and execute binaries using `$PATH`
  - Support for relative and absolute paths
- **Builtins**:
  - `echo` with `-n`
  - `cd` with relative/absolute paths
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- **Environment variables**: `$VAR` expansion
- **Redirections**:
  - `>` output redirection
  - `>>` append output
  - `<` input redirection
  - `<<` heredoc
- **Pipes (`|`)**: chain multiple commands
- **Signals**:
  - `Ctrl-C` → interrupts current command and shows a new prompt
  - `Ctrl-D` → exits the shell
  - `Ctrl-\` → ignored (like in bash)
- **Error handling** with clear messages

---

## ⚙️ Installation & Usage

### Clone the repo
`git clone https://github.com/hassanalmoughabat/MiniShell.git`


`cd minishell`

`make`

This produces the minishell executable.

`./minishell`
You’ll see a prompt like:

`minishell$`
📝 Example

`minishell$ echo Hello World`
`Hello World`

`minishell$ ls -l | grep minishell
-rwxr-xr-x  1 user  staff  41872 Aug 23 14:22 minishell`

`minishell$ export NAME=42
minishell$ echo $NAME
42`
📚 Learning Objectives
Process creation and management (fork, execve, waitpid)

File descriptors & redirections (dup2, pipe)

Signal handling (signal, sigaction)

String parsing and tokenization

Memory management and error handling

🛠️ Development
Language: C

Build system: Makefile

Norm: 42 Norm

`👤 Authors
Hassan AL Mougahabat
Najwa Joudieh`



