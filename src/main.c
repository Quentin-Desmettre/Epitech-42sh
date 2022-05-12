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

static char **parse_input(char *input)
{
    char **word_parse = NULL;
    char *str_separator = ";&|";

    input = clear_str(input);
    input = add_separator(str_separator, input);
    puts(input);
    word_parse = str_to_word_array(input, " ");
    return (word_parse);
}

static char **init_env(char **env)
{
    int len = my_str_array_len(env);
    char **new = malloc(sizeof(char *) * (len + 1));

    new[len] = NULL;
    for (int i = 0; env[i]; i++)
        new[i] = my_strdup(env[i]);
    signal(SIGINT, redirect_sigint);
    return new;
}

int main(int ac, char **av, char **env)
{
    char *input;
    int stop = 0;
    char **tmp = NULL;

    if (ac != 1 || !env || !av)
        return 84;
    env = init_env(env);
    open_stdin();
    while (!stop) {
        set_is_exit(0);
        input = get_shell_input(env, &stop);
        tmp = parse_input(input);
    }
    return get_last_exit();
}
