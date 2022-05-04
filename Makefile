##
## EPITECH PROJECT, 2021
## Makefile
## File description:
## Makefile
##

SRC = \
	src/builtins/cd_get_infos.c                        \
	src/builtins/cd.c                                  \
	src/builtins/env.c                                 \
	src/builtins/exit.c                                \
	src/builtins/setenv.c                              \
	src/builtins/unsetenv.c                            \
	src/globals/last_exit.c                            \
	src/globals/reset_buffer.c                         \
	src/globals/std_in.c                               \
	src/globals/is_exit.c                              \
	src/print_input.c                                  \
	src/redirect.c                                     \
	src/get_input/shell_input.c                        \
	src/get_input/termios.c                            \
	src/main.c                                         \
	src/parse_input/parse_input.c                      \
	src/parse_input/get_next_word.c                    \
	src/parse_input/check_redirections.c               \
	src/parse_input/heredoc.c                          \
	src/parse_input/split_semicolon.c                  \
	src/exec_command/find_command.c                    \
	src/exec_command/exec.c                            \
	src/exec_command/redirects.c                       \
	src/exec_command/pipe.c                            \
	src/command.c                                      \
	src/init_vars.c

OBJ = $(SRC:.c=.o)

NAME = mysh

CFLAGS = -Wall -Wextra -I ./include/ -I lib/my/include/

LDFLAGS += -L lib/my/ -lmy

all: $(NAME)

$(NAME):   $(OBJ)
	make -s -C lib/my/
	gcc -o $(NAME) $(FLAGS) $(OBJ) $(LDFLAGS)

debug: clean
debug: CFLAGS += -g
debug: $(NAME)

clean:
	rm -f $(OBJ)
	find . -name "*.o" -delete
	find . -name "vgcore.*" -delete
	make -s -C lib/my clean

fclean:    clean
	rm -f $(NAME)
	make -s -C lib/my fclean

re:        fclean all
