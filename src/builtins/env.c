/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** env.c
*/

#include "minishell.h"

void cd_change_local(env_t *vars)
{
    char **args = malloc(sizeof(char *) * 3);

    args[0] = malloc(5);
    args[1] = strdup(get_field(vars->env, "PATH="));
    args[2] = NULL;
    strcpy(args[0], "cwd");
    args[1] = strdup(get_field(vars->env, "PWD="));
    set(args, &vars->vars, 1, 0);
    free(args[1]);
    strcpy(args[0], "owd");
    args[1] = strdup(get_field(vars->env, "OLDPWD="));
    set(args, &vars->vars, 1, 0);
    my_free("P", args);
}

int exec_builtin_sec(char **args, env_t *vars, int fds[2], int tmp[2])
{
    int is_pipe = tmp[0];
    int index = tmp[1];

    if (index == 0)
        cd_change_local(vars);
    if (index == 6)
        unalias(args, vars, fds[1], is_pipe);
    if (index == 7)
        setvar_pipe(args, &vars->vars, fds[1], is_pipe);
    if (index == 8)
        unsetvar_pipe(args, &vars->vars, fds[1], is_pipe);
    if (index == 10)
        history_builtin(args, &vars->history, fds[1], is_pipe);
    return 0;
}

int exec_builtin_fd(char **args, command_t *cur, void *params[6], int is_pipe)
{
    void (*builtin[])(char **, char ***, int, int) = {
        &cd_pipe, &setenv_pipe, &unsetenv_pipe,
        NULL, NULL, NULL, NULL, NULL, NULL, &echo_builtin
    };
    env_t *vars = global_env(NULL);
    int *fds = params[0];
    int index = index_str_in_array((char **)builtins, args[0]);

    if ((*(list_t **)(params[5]))->next == *(list_t **)(params[5]) &&
    !HAS_REDIR_OUT(cur) && get_final_fd() >= 0)
        fds[1] = get_final_fd();
    if (index < 3 || index == 9)
        builtin[index](args, &vars->env, fds[1], is_pipe);
    if (index == 3)
        env_pipe(args, &vars->env, fds[1]);
    if (index == 4)
        exit_pipe(args, is_pipe);
    if (index == 5)
        alias(args, vars, fds[1], is_pipe);
    return exec_builtin_sec(args, vars, fds, (int [2]){is_pipe, index});
}

int is_builtin(char const *word)
{
    char *builtins[] = {
        "cd", "setenv", "unsetenv", "env", "exit", "alias", "unalias", "set",
        "unset", "echo", "history", NULL
    };

    for (int i = 0; i < 11; i++)
        if (my_strcmp(word, builtins[i]) == 0)
            return 1;
    return 0;
}

void env_pipe(char **args, char ***e, int o_fd)
{
    char **env = *e;

    if (args[1]) {
        dprint(2, "env: Too many arguments.\n");
        set_last_exit(1);
        return;
    }
    for (int i = 0; env[i]; i++)
        dprint(o_fd, "%s\n", env[i]);
    set_last_exit(0);
}
