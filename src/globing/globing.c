/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** special
*/

#include "minishell.h"
#include <fnmatch.h>
#include <glob.h>

char **get_matching_strings(char const *pattern, char **strings)
{
    char **array = calloc(1, sizeof(char *));

    for (int i = 0; strings[i]; i++) {
        if (!fnmatch(pattern, strings[i], FN_FLAGS))
            append_str_array(&array, strdup(strings[i]));
    }
    return array;
}

char **resolve_globbings(char **args, char **strings)
{
    char **solved = calloc(1, sizeof(char *));
    char **matched;

    for (int i = 1; args[i]; i++) {
        matched = get_matching_strings(args[i], strings);
        for (int i = 0; matched[i]; i++)
            append_str_array(&solved, matched[i]);
        free(matched);
    }
    return solved;
}

char **resolve_unsetenv_globbings(char **args, char **env)
{
    int len = my_str_array_len(env);
    char **splitted_env = malloc(sizeof(char *) * (len + 1));
    char **tmp;
    char **solved;

    splitted_env[len] = NULL;
    for (int i = 0; env[i]; i++) {
        tmp = my_str_to_word_array(env[i], "=");
        splitted_env[i] = tmp[0];
        for (int j = 1; tmp[j]; j++)
            free(tmp[j]);
        free(tmp);
    }
    solved = resolve_globbings(args, splitted_env);
    free_str_array(splitted_env, 0);
    return solved;
}

char **get_glob(char *str)
{
    glob_t glob_buf;

    glob_buf.gl_offs = 0;
    glob(str, GLOB_DOOFFS, NULL, &glob_buf);
    return (glob_buf.gl_pathv);
}

char **place_arr_in_arr(char **dest, char **src, int index)
{
    int size = my_str_array_len(dest);
    int size_src = my_str_array_len(src);
    char **result = malloc(sizeof(char *) * (size + size_src + 1));

    for (int i = 0; i < index && dest[i]; i++)
        result[i] = dest[i];
    for (int i = index; src[i - index]; i++)
        result[i] = src[i - index];
    for (int i = index; dest[i]; i++)
        result[i + size_src] = dest[i + 1];
    result[size + size_src] = NULL;
    return result;
}
