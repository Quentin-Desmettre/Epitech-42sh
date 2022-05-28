/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** find_command.c
*/

#include "minishell.h"

char **get_paths(void)
{
    env_t *vars = global_env(NULL);
    char const *path = get_field(vars->env, "PATH=");
    char const *splitter = ":";
    char **paths;

    if (!path[0]) {
        path = get_field(vars->vars, "path\t");
        splitter = " ()";
    }
    paths = my_str_to_word_array(path, splitter);
    return paths;
}

static char const *get_local_file(char const *str)
{
    char *tmp = my_strdup(str);

    if (access(tmp, F_OK)) {
        free(tmp);
        dprint(2, "%s: Command not found.\n", str);
        return NULL;
    } else if (access(tmp, X_OK)) {
        free(tmp);
        dprint(2, "%s: %s.\n", str, strerror(errno));
        return NULL;
    } else
        return tmp;
    return tmp;
}

static char *check_errors(char **paths,
char *tmp, char *err_mess, char const *str)
{
    free_str_array(paths, 1);
    if (!tmp && err_mess) {
        dprint(2, "%s", err_mess);
    } else if (!tmp)
        dprint(2, "%s: Command not found.\n", str);
    free(err_mess);
    return tmp;
}

char const *find_command(char const *str)
{
    char **paths = get_paths();
    char *tmp = NULL;
    char *err_mess = NULL;

    if (index_of('/', str) >= 0) {
        free_str_array(paths, 1);
        return get_local_file(str);
    }
    for (int i = 0; paths[i]; i++) {
        tmp = str_concat(3, paths[i], "/", str);
        if (!access(tmp, F_OK) && access(tmp, X_OK))
            re_alloc(&err_mess,
            str_concat(4, str, ": ", strerror(errno), ".\n"), 1);
        if (!access(tmp, F_OK) && !access(tmp, X_OK))
            break;
        free(tmp);
        tmp = NULL;
    }
    return check_errors(paths, tmp, err_mess, str);
}

void exec_args(char const *prog, char **args, char **env)
{
    if (execve(prog, args, env) == -1) {
        if (my_strcmp(strerror(errno), "Exec format error") == 0)
            dprint(2, "%s: Exec format error. Wrong Architecture.\n", args[0]);
        else
            dprint(2, "%s: %s.\n", args[0], strerror(errno));
        free((char *)prog);
        free_str_array(args, 1);
        free_str_array(env, 1);
        exit(1);
    }
}
