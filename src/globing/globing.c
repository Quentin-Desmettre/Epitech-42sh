/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** special
*/

#include "minishell.h"
#include <fnmatch.h>
#include <glob.h>

static void delete_by_in_str(char *str, char to_delete, char to_replace)
{
    int i;

    if (!contain(str, to_delete))
        append_char(&str, to_delete, 1);
    for (i = 0; str[i] != to_delete; i++);
    str[i] = to_replace;
}

void check_glob_unalias(char ***args, list_t *commands)
{
    char **tmp = calloc(1, sizeof(char *));
    int res = 0;
    int moove = 0;
    list_t *save = commands;
    replace_t *test = NULL;

    do {
        test = save->data;
        puts(test->name);
        save = save->next;
    } while (save != commands && save);

    printf("coucou\n");
}

void check_glob_unsetenv(char ***args, char **e)
{
    char **tmp = calloc(1, sizeof(char *));
    int res = 0;
    int moove = 0;
    for (int i = 0; e[i]; i++)
        delete_by_in_str(e[i], '=', '\0');
    for (int i = 1; (*args)[i]; i++) {
        for (int j = 0; e[j]; j++) {
            res = fnmatch((*args)[i], e[j], 0);
            !res ? append_str_array(&tmp, strdup(e[j])) : 0;
        }
        if (tmp) {
            moove = my_str_array_len(tmp);
            (*args) = place_arr_in_arr((*args), tmp, i);
            i += moove;
            tmp = NULL;
            tmp = calloc(1, sizeof(char *));
        }
    }
    for (int i = 0; e[i]; i++)
        delete_by_in_str(e[i], '\0', '=');
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
