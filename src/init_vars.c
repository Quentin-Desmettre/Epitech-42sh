/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** init_vars.c
*/

#include "minishell.h"

static char **init_local(void)
{
    char **vars = malloc(sizeof(char *) * 1);

    vars[0] = NULL;
    return vars;
}

aliases_t *init_alias(void)
{
    aliases_t *aliases = malloc(sizeof(aliases_t));

    aliases->commands = NULL;
    return (aliases);
}

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

    my_memset(vars, 0, sizeof(env_t));
    vars->env = init_env(env);
    vars->aliases = init_alias();
    vars->vars = init_local();
    open_stdin();
    return vars;
}
