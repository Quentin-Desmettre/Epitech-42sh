/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** out_fd.c
*/

#include "minishell.h"

int output_fd(int change, int new_val)
{
    static int fd = -1;

    if (change)
        fd = new_val;
    return fd;
}

void set_final_fd(int fd)
{
    output_fd(1, fd);
}

int get_final_fd(void)
{
    return output_fd(0, 0);
}
