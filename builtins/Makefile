NAME = builtins_lib.a

#CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = ../libft
LIBFT = ../libft/libft.a

SOURCES = cd.c\
		util_funcs.c\
		echo.c\
		env.c\
		exit.c\
		export.c\
		pwd.c\
		search_linked_list.c\
		unset.c\
		utils.c\

OBJECTS = $(SOURCES:.c=.o)

all: $(NAME)

$(NAME) : $(OBJECTS) $(LIBFT)
	ar rcs $(CFLAGS) $(NAME) $(OBJECTS) $(LIBFT)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJECTS)
	make clean -C $(LIBFT_DIR) 

fclean: clean
	rm -rf $(NAME)
	rm -rf $(LIBFT)

re: fclean all

.PHONY: all, clean, fclean, re
