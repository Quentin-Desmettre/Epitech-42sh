/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** exec.c
*/

#include "minishell.h"

static void wait_pids(int *pids, int size, int cannot_find)
{
    int status = 0;
    int has_crashed = 0;

    for (int j = 0; j < size; j++)
        if (pids[j]) {
            waitpid(pids[j], &status, 0);
            check_exit_status_fd(status, &has_crashed);
        }
    if (has_crashed)
        write(2, "\n", 1);
    if (cannot_find)
        set_last_exit(1);
    free(pids);
}

static int *init_pids(int size)
{
    int *pids = malloc(sizeof(int) * size);

    my_memset(pids, 0, sizeof(int) * size);
    signal(SIGINT, SIG_IGN);
    return pids;
}

static void clear_cmds(command_t **cur, int fds[2], list_t **commands)
{
    if (*commands)
        *cur = (*commands)->data;
    fds[0] = STDIN_FILENO;
    fds[1] = STDOUT_FILENO;
}

static void clear_childs(int *pids, int i, list_t **commands)
{
    set_last_exit(1);
    kill_childs(pids, i);
    destroy_list(commands, free_command);
    free(pids);
}

void exec_commands(list_t *commands, env_t *vars, list_t **all_commands)
{
    command_t *cur;
    int fds[2] = {STDIN_FILENO, STDOUT_FILENO};
    int size = list_size(commands);
    int *pids = init_pids(size);
    int is_pipe = 0;
    int cannot_find = 0;
    void *params[6] = {fds, pids, &cannot_find, vars, all_commands, &commands};

    for (int i = 0; commands; i++) {
        clear_cmds(&cur, fds, &commands);
        get_output_fd(cur, fds, &is_pipe, commands);
        if (get_input_fd(cur, fds) == MAJOR_ERROR ||
        exec_command(cur, params, is_pipe, i) == MAJOR_ERROR)
            return clear_childs(pids, i, &commands);
        close_pipe(fds);
        is_pipe = 0;
        remove_node(&commands, 0, free_command);
    }
    wait_pids(pids, size, cannot_find);
}
