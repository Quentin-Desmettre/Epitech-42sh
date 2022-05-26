##
## EPITECH PROJECT, 2021
## Makefile
## File description:
## Makefile
##

SRC = \
	iomanip/my_getnbr.c                         \
	iomanip/my_put_nbr.c                        \
	iomanip/my_putchar.c                        \
	iomanip/my_putstr.c                         \
	iomanip/my_show_word_array.c                \
	linked_list/append_node.c                   \
	my_printf/src/casts/floats.c                \
	my_printf/src/casts/get_raw_str.c           \
	my_printf/src/casts/signed_casts.c          \
	my_printf/src/casts/string_casts.c          \
	my_printf/src/casts/unsigned_casts.c        \
	my_printf/src/casts/var_to_str.c            \
	my_printf/src/formats/format.c              \
	my_printf/src/formats/format_flags.c        \
	my_printf/src/formats/format_precision.c    \
	my_printf/src/formats/format_width.c        \
	my_printf/src/get_formats/get_flags.c       \
	my_printf/src/get_formats/get_format.c      \
	my_printf/src/get_formats/get_params.c      \
	my_printf/src/print/my_printf.c             \
	my_printf/src/print/show_non_print.c        \
	others/my_sort_int_array.c                  \
	others/my_sort_word_array.c                 \
	others/my_swap.c                            \
	others/my_free.c                            \
	strmanip/append_char.c                      \
	strmanip/contain.c                          \
	strmanip/my_memset.c                        \
	strmanip/my_revstr.c                        \
	strmanip/my_str_to_word_array.c             \
	strmanip/my_strcat.c                        \
	strmanip/my_strcmp.c                        \
	strmanip/my_strcpy.c                        \
	strmanip/my_strdup.c                        \
	strmanip/my_strlen.c                        \
	strmanip/my_strncat.c                       \
	strmanip/my_strncmp.c                       \
	strmanip/my_strncpy.c                       \
	strmanip/my_strstr.c                        \
	strmanip/replace_str.c                      \
	strmanip/str_array_contain.c                \
	strmanip/str_is_num.c                       \
	strmanip/strconcat.c                        \
	my_malloc/my_exit.c                         \
	my_malloc/my_malloc.c

OBJ = $(SRC:.c=.o)

NAME = libmy.a

CFLAGS = -Wall -Wextra -I ./include/

all: $(NAME)

$(NAME):   $(OBJ)
	ar rc $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean:    clean
	rm -f $(NAME)

re:        fclean all
