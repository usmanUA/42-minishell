# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mkorpela <mkorpela@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/08 14:29:18 by mkorpela          #+#    #+#              #
#    Updated: 2024/03/14 16:35:46 by mkorpela         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CFLAGS = -Wall -Wextra -Werror

SOURCES =	get_next_line_utils.c\
			get_next_line.c\
			ft_atoi.c\
			ft_bzero.c\
			ft_calloc.c\
			ft_isalnum.c\
			ft_isalpha.c\
			ft_isascii.c\
			ft_isdigit.c\
			ft_isprint.c\
			ft_isspace.c \
			ft_ispresent.c \
			ft_itoa.c\
			ft_lstadd_back_bonus.c\
			ft_lstadd_front_bonus.c\
			ft_lstclear_bonus.c\
			ft_lstdelone_bonus.c\
			ft_lstiter_bonus.c\
			ft_lstlast_bonus.c\
			ft_lstmap_bonus.c\
			ft_lstnew_bonus.c\
			ft_lstsize_bonus.c\
			ft_memchr.c\
			ft_memcmp.c\
			ft_memcpy.c\
			ft_memmove.c\
			ft_memset.c\
			ft_putchar_fd.c\
			ft_putendl_fd.c\
			ft_putnbr_fd.c\
			ft_putstr_fd.c\
			ft_split.c\
			ft_strchr.c\
			ft_strdup.c\
			ft_striteri.c\
			ft_strjoin.c\
			ft_strlcat.c\
			ft_strlcpy.c\
			ft_strlen.c\
			ft_strmapi.c\
			ft_strncmp.c\
			ft_strnstr.c\
			ft_strrchr.c\
			ft_strtrim.c\
			ft_substr.c\
			ft_tolower.c\
			ft_toupper.c\
			convert_long_to_hex.c\
			format_specifier_logic.c\
			ft_printf.c\
			int_putchar.c\
			int_putnbr.c\
			int_putstr.c\
			uint_to_lowercase_hex.c\
			uint_to_uppercase_hex.c\
			unsigned_int_putnbr.c\

OBJECTS = $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	ar rcs $(NAME) $(OBJECTS)

clean:
	rm -f ${OBJECTS}

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:	all, clean, fclean, re
