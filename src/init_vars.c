/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** init_vars.c
*/

#include "minishell.h"

static char **init_env(char **env)
{
    int len = my_str_array_len(env);
    char **new = malloc(sizeof(char *) * (len + 1));

    new[len] = NULL;
    for (int i = 0; env[i]; i++)
        new[i] = my_strdup(env[i]);
    return new;
}

void free_vars(env_t *vars)
{
    free_str_array(vars->env, 0);
}

env_t *init_vars(char **env)
{
    env_t *vars = malloc(sizeof(env_t));

    memset(vars, 0, sizeof(env_t));
    vars->env = init_env(env);
    return vars;
}
