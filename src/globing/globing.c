/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** special
*/

#include "minishell.h"
#include <glob.h>

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
    free(dest[index]);
    for (int i = index; src[i - index]; i++)
        result[i] = src[i - index];
    for (int i = index; dest[i]; i++)
        result[i + size_src] = dest[i + 1];
    result[size + size_src] = NULL;
    free(dest);
    free(src);
    return result;
}
