/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** pipe.c
*/

#include "minishell.h"

void close_pipe(int fds[2])
{
    if (fds[0] != 0)
        close(fds[0]);
    if (fds[1] != 1)
        close(fds[1]);
}

int redirect_pipe(int fds[2])
{
    if (dup2(fds[0], 0) < 0 || dup2(fds[1], 1) < 0) {
        if (fds[0] >= 0)
            dprint(2, "dup2: %s.\n", strerror(errno));
        return 0;
    }
    if (fds[0] != 0)
        close(fds[0]);
    if (fds[1] != 1)
        close(fds[1]);
    return 1;
}

void kill_childs(int *pids, int size)
{
    for (int i = 0; i < size; i++)
        if (pids[i])
            kill(pids[i], SIGKILL);
}
