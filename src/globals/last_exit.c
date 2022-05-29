/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** last_exit.c
*/

#include "minishell.h"

int last_exit(int new_status, int val)
{
    static int last = 0;

    if (new_status)
        last = val;
    return last;
}

void set_last_exit(int status)
{
    env_t *vars = global_env(NULL);
    char *val = nbr_to_str(status);

    last_exit(1, status);
    var_setter(&vars->vars,
    var_index_of_key(vars->vars, "?"), (char *[]){"?", val, NULL});
    free(val);
}

int get_last_exit(void)
{
    return last_exit(0, 0);
}
