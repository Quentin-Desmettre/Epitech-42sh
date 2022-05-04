/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** redirect.c
*/

#include "minishell.h"

void redirect_sigint(int id)
{
    if (id != SIGINT)
        return;
    close_stdin();
    set_reset_buffer(1);
    set_last_exit(1);
    write(1, "\n", 1);
}
