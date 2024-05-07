flags = -Wall -Wextra -Werror
NAME = libvec.a

FILES = \
	vec_append.c \
	vec_copy.c \
	vec_filter.c \
	vec_free.c \
	vec_from.c \
	vec_get.c \
	vec_insert.c \
	vec_iter.c \
	vec_map.c \
	vec_new.c \
	vec_pop.c \
	vec_prepend.c \
	vec_push.c \
	vec_reduce.c \
	vec_remove.c \
	vec_resize.c \
	vec_sort.c \

OBJS = $(FILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Creating Library libvec.a"
	@ar rc $(NAME) $^

%.o: %.c
	@echo "Creating $@ Object File"
	@cc $(flags) -c $< -o $@

clean:
	@echo "Cleaning $(OBJS)"
	@rm -rf $(OBJS)

fclean: clean
	@echo "Removing $(NAME)"
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
