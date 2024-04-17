flags = -Wall -Wextra -Werror
LIB = .lib.a
LIBFT = libft.a
NAME = pipex
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

FILES = pipex.c \
		exec_cmds.c \

B_FILES = pipex_bonus.c \
		  exec_cmds_bonus.c \
		  get_next_line/get_next_line.c \
		  get_next_line/get_next_line_utils.c \

LIB_FILES = check_input.c \
			exec_cmds_utils.c \
			errors.c \
			path.c \
			pipex_utils.c \


OBJS = $(FILES:.c=.o)

LIB_OBJS = $(LIB_FILES:.c=.o)

B_OBJS = $(B_FILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIB) $(LIBFT)
	@(echo "$(GREEN)Creating The Executable:   $(NAME)$(RESET)" && cc $(OBJS) $(LIB) libft/$(LIBFT) -o $(NAME))
	
%.o: %.c
	@(echo "$(GREEN)Creating Object File: $@$(RESET)" && cc $(flags) -c $< -o $@)
	
bonus: $(B_OBJS) $(LIB) $(LIBFT)
	@(echo "$(GREEN)Creating The Executable:   $(NAME)$(RESET)" && cc $(B_OBJS) $(LIB) libft/$(LIBFT) -o $(NAME))

$(LIB): $(LIB_OBJS)
	@ar rc $(LIB) $^

$(LIBFT):
	@$(MAKE) -C libft

clean:
	@echo "$(RED)Cleaning Object Files for libft and push_swap$(RESET)"
	@$(MAKE) -C libft clean
	@rm -rf $(OBJS) $(B_OBJS) $(LIB_OBJS)

fclean: clean
	@echo "$(RED)Removing   $(LIBFT) and $(NAME)$(RESET)"
	@rm -rf libft/$(LIBFT) $(LIB)
	@rm -rf $(NAME) $(B_NAME)

re: fclean all

.PHONY: all clean fclean re
