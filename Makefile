flags = -Wall -Wextra -Werror
READLINE_LIB = ~/.brew/Cellar/readline/8.2.10/lib
READLINE_HEADER = ~/.brew/Cellar/readline/8.2.10/include
LIBVEC = vec/libvec.a
LIBFT = libft/libft.a
NAME = minishell
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

FILES = \
			main.c \
			envp.c \
			errors.c \
			execute.c \
			free_mem.c \
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
			get_next_line/get_next_line.c \
			get_next_line/get_next_line_utils.c \

OBJS = $(FILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBVEC) $(LIBFT)
	@(echo "$(GREEN)Creating The Executable:   $(NAME)$(RESET)" && cc $(OBJS) $(LIBVEC) $(LIBFT) -L ${READLINE_LIB} -lreadline -I ${READLINE_HEADER} -g -fsanitize=address -o $(NAME))
	
%.o: %.c
	@(echo "$(GREEN)Creating Object File: $@$(RESET)" && cc $(flags) -I ${READLINE_HEADER} -c $< -o $@)
	
$(LIBFT):
	@$(MAKE) -C libft

$(LIBVEC):
	@$(MAKE) -C vec

clean:
	@echo "$(RED)Cleaning Object Files for libft and push_swap$(RESET)"
	@$(MAKE) -C libft clean
	@$(MAKE) -C vec clean
	@rm -rf $(OBJS) 

fclean: clean
	@echo "$(RED)Removing   $(LIBFT) and $(NAME)$(RESET)"
	@rm -rf $(LIBFT) $(LIBVEC)
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
