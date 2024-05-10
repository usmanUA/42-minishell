flags = -Wall -Wextra -Werror
READLINE_LIB = ~/.brew/Cellar/readline/8.2.10/lib
READLINE_HEADER = ~/.brew/Cellar/readline/8.2.10/include
LIBVEC = vec/libvec.a
BUILTIN_LIB = builtins/builtins_lib.a
LIBFT = libft/libft.a
NAME = minishell
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

FILES = \
			main.c \
			make_linked_list_and_utils.c \
			errors.c \
			errors_2.c \
			execute.c \
			free_mem.c \
			free.c \
			init_structs.c \
			minishell_utils.c \
			next_string.c \
			next_string_utils.c \
			vars_expansion.c \
			parsing_begins.c \
			parsing_commands.c \
			parsing_redirects.c \
			print_vecs.c \
			signals.c \
			syntax_check.c \
			syntax_utils.c \
			validate_and_execute.c \
			validate_commands.c \

OBJS = $(FILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBVEC) $(BUILTIN_LIB) $(LIBFT)
	@(echo "$(GREEN)Creating The Executable:   $(NAME)$(RESET)" && cc $(OBJS) $(LIBVEC) $(BUILTIN_LIB) $(LIBFT) -L ${READLINE_LIB} -lreadline -I ${READLINE_HEADER} -g -fsanitize=address -o $(NAME))
	
%.o: %.c
	@(echo "$(GREEN)Creating Object File: $@$(RESET)" && cc $(flags) -I ${READLINE_HEADER} -c $< -o $@)
	
$(BUILTIN_LIB):
	@$(MAKE) -C builtins

$(LIBVEC):
	@$(MAKE) -C vec

$(LIBFT):
	@$(MAKE) -C libft

clean:
	@echo "$(RED)Cleaning Object Files for builtins and push_swap$(RESET)"
	@$(MAKE) -C builtins clean
	@$(MAKE) -C vec clean
	@$(MAKE) -C libft clean
	@rm -rf $(OBJS) 

fclean: clean
	@echo "$(RED)Removing   $(BUILTIN_LIB) and $(NAME)$(RESET)"
	@rm -rf $(BUILTIN_LIB) $(LIBVEC) $(LIBFT)
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
