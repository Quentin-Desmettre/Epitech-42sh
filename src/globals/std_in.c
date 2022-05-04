/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** std_in.c
*/

#include "minishell.h"

int std_in(int new_val, int new)
{
    static int std = -1;

    if (new_val)
        std = new;
    return std;
}

void open_stdin(void)
{
    int fd = open("/dev/stdin", O_RDONLY);

    if (fd < 0)
        exit(!dprint(2, "error: cannot open standard input.\n") + 84);
    std_in(1, fd);
}

int get_stdin(void)
{
    return std_in(0, 0);
}

void close_stdin(void)
{
    if (close(get_stdin()))
        exit(dprint(2, "error: cannot close standard input.\n") ? 84 : 84);
    std_in(1, -1);
}
