/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** env.c
*/

#include "minishell.h"

env_t *global_env(env_t *new)
{
    static env_t *glob = 0;

    if (new)
        glob = new;
    return glob;
}
