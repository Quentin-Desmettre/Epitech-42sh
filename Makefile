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
	src/builtins/alias.c                               \
	src/builtins/unalias.c                             \
	src/builtins/history.c                             \
	src/builtins/set.c                            	   \
	src/builtins/set2.c                            	   \
	src/builtins/set3.c                            	   \
	src/builtins/unset.c                               \
	src/builtins/echo.c                                \
	src/globals/last_exit.c                            \
	src/globals/free_stdin.c                           \
	src/globals/reset_buffer.c                         \
	src/globals/std_in.c                               \
	src/globals/is_exit.c                              \
	src/globals/out_fd.c 							   \
	src/globals/env.c                                  \
	src/print_input.c                                  \
	src/get_input/shell_input.c                        \
	src/get_input/special.c                            \
	src/get_input/arrows.c                             \
	src/get_input/termios.c                            \
	src/get_input/tab_globing.c                        \
	src/get_input/tab_globing_print.c                  \
	src/get_input/remove_string_from_array.c           \
	src/main.c                                         \
	src/parse_input/heredoc.c                          \
	src/parse_input/history_input.c                    \
	src/parse_input/split_semicolon.c                  \
	src/exec_command/find_command.c                    \
	src/exec_command/exec.c                            \
	src/exec_command/redirects.c                       \
	src/exec_command/pipe.c                            \
	src/command.c                                      \
	src/parse_input/clear_input_str.c                  \
	src/parse_input/edit_input.c                       \
	src/parse_input/str_to_word_array_input.c          \
	src/backticks/backticks.c                          \
	src/globing/globing.c                              \
	src/init_vars.c									   \
	src/history/init_history.c                         \
	src/history/history_manip.c                        \
	src/parse_input/replace_variable.c				   \
	src/parse_input/delete_backslash.c				   \
	src/parse_input/aliases_in_parse.c				   \
	src/parse_input/check_exec_manip.c 	 	  	 	   \
	src/parse_input/call_all_check.c				   \
	src/parse_input/generate_command.c				   \
	src/parse_input/globing.c						   \
	src/exec_file.c

OBJ = $(SRC:.c=.o)

NAME = 42sh

CFLAGS = -Wall -Wextra -I ./include/ -I lib/my/include/

LDFLAGS += -L lib/my/ -lmy

all: $(NAME)

$(NAME): $(OBJ)
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
