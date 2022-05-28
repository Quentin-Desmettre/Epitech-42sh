/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** print_input.c
*/

#include "minishell.h"

char const *get_field(char **env, char const *field)
{
    int f_len = my_strlen(field);

    for (int i = 0; env[i]; i++)
        if (my_strncmp(env[i], field, f_len) == 0)
            return env[i] + f_len;
    return "";
}

static char *get_raw_dir(void)
{
    char *dir = get_current_dir();
    int last_slash = 0;
    char **tmp;

    if (!dir)
        return "";
    for (int i = 1; dir[i]; i++) {
        if (dir[i] == '/')
            last_slash = i;
    }
    tmp = my_str_to_word_array(dir, "/");
    re_alloc(&dir, my_strdup(dir + last_slash + (tmp[1] ? 1 : 0)), 1);
    free_str_array(tmp, 1);
    return dir;
}

char *get_prompt(char **env)
{
    char const *user = get_field(env, "USER=");
    char const *host = get_field(env, "HOSTNAME=");
    char *dir = get_raw_dir();
    char *final;

    if (host[0] == 0)
        host = get_field(env, "HOST=");
    if (my_strcmp(user, dir) == 0)
        re_alloc(&dir, my_strdup("~"), 1);
    final = str_concat(7, "[", user, "@", host, " ", dir, "]$ ");
    return final;
}
