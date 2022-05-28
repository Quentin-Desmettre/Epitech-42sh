/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** setenv.c
*/

#include "minishell.h"

void var_pipe(char ***e, int o_fd)
{
    char **env = *e;

    for (int i = 0; env[i]; i++) {
        dprint(o_fd, "%s\n", env[i]);
    }
    set_last_exit(0);
}

char **set_arr_var(char **args)
{
    char **arr = malloc(sizeof(char *) * my_str_array_len(args));
    int test = 0;

    for (int i = 0; i != my_str_array_len(args); i++)
        arr[i] = 0;
    for (int i = 1; i < my_str_array_len(args); i++) {
        if (args[i + 1] && args[i + 2] && strcmp(args[i + 1], "=") == 0) {
            arr[test++] = str_concat(3, args[i], args[i + 1], args[i + 2]);
            i += 2;
            continue;
        }
        if (args[i + 1] && (args[i + 1][0] == '=' || args[i][strlen(args[i])
        - 1] == '=')) {
            arr[test++] = str_concat(2, args[i], args[i + 1]);
            i++;
            continue;
        }
        arr[test++] = strdup(args[i]);
    }
    return arr;
}

char **loop_call_set(int i, char **arr)
{
    char **array = malloc(sizeof(char *) * 3);
    int tmp = -1;

    array[2] = NULL;
    for (int j = 0; arr[i][j]; j++)
        if (arr[i][j] == '=') {
            tmp = j;
            break;
        }
    if (tmp != -1) {
        array[0] = malloc(sizeof(char) * (tmp + 1));
        memset(array[0], 0, tmp + 1);
        array[0] = strncpy(array[0], arr[i], tmp);
        array[1] = malloc(sizeof(char) * (strlen(arr[i]) - tmp + 1));
        array[1] = strcpy(array[1], arr[i] + tmp + 1);
    } else {
        array[0] = strdup(arr[i]);
        array[1] = 0;
    }
    return array;
}

char *get_path(char **env)
{
    char *path = strdup(get_field(env, "PATH="));

    if (!path[0] || strcmp(path, "/") == 0) {
        free(path);
        path = strdup("/usr/bin /bin");
    }
    for (int i = 0; path[i]; i++)
        path[i] = path[i] == ':' ? ' ' : path[i];
    return path;
}
