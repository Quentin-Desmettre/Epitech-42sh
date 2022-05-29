/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** exec_file.c
*/

#include "minishell.h"

static void exec_loop(int fd, env_t *vars, int stop, char *input)
{
    int stdin_copy = dup(0);

    dup2(fd, 0);
    stdin = fdopen(0, "r");
    close(fd);
    while (!stop) {
        set_reset_buffer(0);
        set_is_exit(0);
        if (!(input = get_shell_input(vars, &stop)))
            break;
        start_parsing(input, vars, 0);
    }
    fclose(stdin);
    dup2(stdin_copy, 0);
    close(stdin_copy);
    stdin = fdopen(0, "r");
    set_free_stdin(1);
}

void exec_file(char const *file, int is_rc)
{
    env_t *vars = global_env(NULL);
    int fd = open(file, O_RDONLY);

    if (fd < 0 && !is_rc)
        return (void)dprint(2, "%s: %s.\n", file, strerror(errno));
    if (fd < 0)
        return;
    exec_loop(fd, vars, 0, NULL);
}

void exec_files(int nb, char **files)
{
    for (int i = 0; i < nb; i++)
        exec_file(files[i], 0);
}
