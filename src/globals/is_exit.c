/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** is_exit.c
*/

#include "minishell.h"

int is_exit_glob(int change, int new_val)
{
    static int is_ex = 0;

    if (change)
        is_ex = new_val;
    return is_ex;
}
