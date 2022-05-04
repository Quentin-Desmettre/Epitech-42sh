/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** exit.c
*/

#include "minishell.h"

static int exit_status(int change, int new)
{
    static int status = 0;

    if (change)
        status = new;
    return status;
}

void set_exit_status(int val)
{
    exit_status(1, val);
}

int get_exit_status(void)
{
    return exit_status(0, 0);
}

static int check_args(char **args)
{
    if (!args[1])
        return 1;
    if (args[1][0] == '-') {
        if (!contain_only(args[1] + 1, "0123456789"))
            return dprint(2, "exit: Badly formed number.\n") ? 0 : 0;
        if (args[2])
            return dprint(2, "exit: Expression Syntax.\n") ? 0 : 0;
        return 1;
    }
    if (!contain("0123456789", args[1][0]))
        return dprint(2, "exit: Expression Syntax.\n") ? 0 : 0;
    if (!contain_only(args[1], "0123456789"))
        return dprint(2, "exit: Badly formed number.\n") ? 0 : 0;
    if (args[2])
        return dprint(2, "exit: Expression Syntax.\n") ? 0 : 0;
    return 1;
}

void exit_pipe(char **args, int is_pipe)
{
    int is_error = 0;
    int code;

    if (is_pipe)
        return;
    if (!check_args(args)) {
        set_last_exit(1);
        return;
    }
    code = args[1] ? my_getnbr(args[1], &is_error) : get_last_exit();
    if (is_error) {
        dprint(2, "exit: Expression Syntax.\n");
        set_last_exit(1);
        return;
    }
    set_is_exit(1);
    set_exit_status(code);
}
