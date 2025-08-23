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
```bash
git clone https://github.com/hassanalmoughabat/MiniShell.git
