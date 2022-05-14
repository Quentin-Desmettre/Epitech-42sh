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

static void parse_input(char const *input, env_t *vars)
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
        exec_commands(cmds, vars, &commands);
    }
    if (is_exit()) {
        free_vars(vars);
        write(2, "exit\n", 5);
        exit(get_last_exit());
    }
}

int main(int ac, char **av, char **env)
{
    char *input;
    int stop = 0;
    struct termios saved_term;
    env_t *vars;

    if (ac != 1 || !env || !av)
        return 84;
    vars = init_vars(env);
    open_stdin();
    tcgetattr(0, &saved_term);
    original_termios(&saved_term);
    write(1, "\33[s", 3);
    while (!stop) {
        set_is_exit(0);
        input = get_shell_input(vars, &stop);
        parse_input(input, vars);
    }
    return get_last_exit();
}
