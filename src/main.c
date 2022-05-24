/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** main.c
*/

#include "minishell.h"

int main(int ac, char **av, char **env)
{
    char *input;
    int stop = 0;
    struct termios saved_term;
    env_t *vars;

    if (ac != 1 || !env || !av)
        return 84;
    vars = init_vars(env);
    tcgetattr(0, &saved_term);
    original_termios(&saved_term);
    while (!stop) {
        set_is_exit(0);
        input = get_shell_input(vars, &stop);
        history_append(input, vars->history);
        if (!input)
            break;
        if (input[0] && parse_for_backticks(&input, vars))
            new_parse_input(input, vars);
        else
            free(input);
    }
    return 0;
}
