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

static void parse_input(char const *input, char ***env)
{
    list_t *commands = split_semicolons(input);
    list_t *cmds;
    char *err_mess;

    free((char *)input);
    while (commands) {
        cmds = get_command_list(commands->data, &err_mess);
        remove_node(&commands, 0, free);
        if (!cmds)
            dprint(2, "%s\n", err_mess);
        if (!cmds && flush_stdin(err_mess))
            return destroy_list(&commands, free);
        exec_commands(cmds, env, &commands);
    }
    if (is_exit()) {
        free_str_array(*env, 1);
        write(2, "exit\n", 5);
        exit(get_last_exit());
    }
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

    if (ac != 1 || !env || !av)
        return 84;
    env = init_env(env);
    open_stdin();
    while (!stop) {
        set_is_exit(0);
        input = get_shell_input(env, &stop);
        parse_input(input, &env);
    }
    return get_last_exit();
}
