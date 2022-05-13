/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** main.c
*/

#include "minishell.h"

static int flush_stdin(char *err_mess)
{
    char t;

    if (my_strcmp(err_mess, NULL_CMD))
        return 0;
    if (!isatty(get_stdin()))
        while (read(get_stdin(), &t, 1) == 1);
    close_stdin();
    open_stdin();
    return 1;
}

void new_parse_input(char const *input, env_t *vars);

int main(int ac, char **av, char **env)
{
    char *input;
    int stop = 0;
    struct termios saved_term;
    env_t *vars;
    char **tmp;

    if (ac != 1 || !env || !av)
        return 84;
    vars = init_vars(env);
    open_stdin();
    tcgetattr(0, &saved_term);
    original_termios(&saved_term);
    while (!stop) {
        set_is_exit(0);
        input = get_shell_input(vars, &stop);
        new_parse_input(input, vars);
    }
    return 0;
}
