# Minishell

## Description

A miniature version of bash.

## About

- 42-minishell can execute commands from absolute (`/bin/cat`), relative (`../../../../../bin/cat`), or environment PATH (`cat`) with arguments.
- Handles environment variables and exit status (`$?`).
- Supports redirections (`> >> < <<`) and pipes (`|`).
- Built-in commands: `cd`, `pwd`, `echo`, `export`, `unset`, `env`, `exit`.
- Manages single and double quotes like bash.
- Handles signals:
  - `Ctrl-C` displays a new prompt.
  - `Ctrl-\` does nothing.
  - `Ctrl-D` exits the shell.


## Functionality

- When the program starts, it saves the environment variables into a linked list.
- The parser reads the command-line input and dynamically allocates memory for C vectors based on user input.
- Unlike bash, minishell does not build an AST tree; it handles variable expansions, quotes, and command parsing directly.
- After parsing, command validation is performed, and if valid, the commands are executed.


