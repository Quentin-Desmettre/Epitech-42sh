/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** free_stdin.c
*/

#include "minishell.h"

int free_stdin(int new_val, int new)
{
    static int std = 0;

    if (new)
        std = new_val;
    return std;
}

void set_free_stdin(int new_val)
{
    free_stdin(new_val, 1);
}

int is_free_stdin(void)
{
    return free_stdin(0, 0);
}
