/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** setvar.c
*/

#include "minishell.h"

int place_in_local(char **new, char ***array, int i, char *val)
{
    char *tmp = 0;
    int j = 0;
    int cmp = 0;

    new[i] = (*array)[i];
    if ((*array)[i + 1]) {
        tmp = my_strdup((*array)[i + 1]);
        for (j = 0; tmp[j] != '\t'; j++);
        tmp[j] = '\0';
        cmp = my_strcmp(val, tmp);
        free(tmp);
    }
    return cmp;
}

void append_var_array(char ***array, char *val)
{
    int i = 0;
    char **new = malloc(sizeof(char *) * (my_str_array_len(*array) + 2));
    int cmp;
    char *tmp;

    if ((*array)[0]) {
        tmp = my_strdup((*array)[0]);
        cmp = my_strcmp(val, tmp);
        free(tmp);
        for (; ((*array)[i]) && cmp > 0; i++)
            cmp = place_in_local(new, array, i, val);
    }
    new[i] = val;
    for (; (*array)[i]; i++)
        new[i + 1] = (*array)[i];
    new[i + 1] = NULL;
    free(*array);
    *array = new;
}

void set_var(char ***var, int index, char const *key, char const *val)
{
    char *buf = "";

    append(&buf, key, 0);
    append(&buf, "\t", 1);
    if (val)
        append(&buf, val, 1);
    free((*var)[index]);
    (*var)[index] = buf;
}

void create_var(char ***var, char const *key, char const *val)
{
    char *buf = "";

    append(&buf, key, 0);
    append(&buf, "\t", 1);
    if (val)
        append(&buf, val, 1);
    append_var_array(var, buf);
}

int var_args_valid(char **args)
{
    int is_error;

    if (!args[1])
        return 0;
    my_getnbr(args[1], &is_error);
    return 1;
}
