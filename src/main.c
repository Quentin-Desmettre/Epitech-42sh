/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** main.c
*/

#include "minishell.h"

void continue_input_tty(char **in, env_t *vars)
{
    char *tmp = NULL;
    char *input = *in;

    while (input[strlen(input) - 1] == '\\') {
        tmp = get_next_line(NULL);
        if (!tmp || !tmp[0]) {
            free(tmp);
            break;
        }
        input[strlen(input) - 1] = 0;
        append(&input, tmp, 1);
        free(tmp);
    }
    *in = input;
}

void continue_input(char **in, env_t *vars)
{
    char *tmp;
    char *input = *in;

    if (!isatty(0))
        return continue_input_tty(in, vars);
    while (input[strlen(input) - 1] == '\\') {
        write(1, "\33[s", 3);
        tmp = get_command(NULL, vars->env, "? ", vars->history);
        if (!tmp)
            break;
        if (!tmp[0] && is_reset_buf()) {
            is_reset_buf() ? input[0] = 0 : 0;
            free(tmp);
            break;
        }
        input[strlen(input) - 1] = 0;
        append(&input, tmp, 1);
        free(tmp);
    }
    *in = input;
}

void start_parsing(char *input, env_t *vars)
{

    continue_input(&input, vars);
    history_append(input, vars->history);
    if (input[0] && parse_for_backticks(&input, vars))
        new_parse_input(input, vars);
    else
        free(input);
}

int main(int ac, char **av, char **env)
{
    char *input;
    int stop = 0;
    struct termios saved_term;
    env_t *vars;

    if (ac != 1 || !env || !av)
        return 84;
    vars = init_vars(env, &saved_term);
    global_env(vars);
    while (!stop) {
        set_reset_buffer(0);
        set_is_exit(0);
        if (!(input = get_shell_input(vars, &stop)))
            break;
        start_parsing(input, vars);
    }
    exit(0);
}
