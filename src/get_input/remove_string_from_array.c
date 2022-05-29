/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** shell_print.c
*/

#include "minishell.h"

void remove_string_arr(char **arr, int nb)
{
    free(arr[nb]);
    for (int i = nb; arr[i]; i++) {
        arr[i] = arr[i + 1];
    }
}
