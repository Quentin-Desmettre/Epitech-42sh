/*
** EPITECH PROJECT, 2022
** TMP_C
** File description:
** call_all_check.c
*/

#include "minishell.h"

int check_everything(command_t **tmp, char **all[2], int *i, list_t **commands)
{
    int status;

    if (!(status = check_pipe(tmp, all, i, commands)) ||
    (status == NOTHING_DONE &&
    !(status = check_redir_out(*tmp, all[0], all[1], i))) ||
    (status == NOTHING_DONE &&
    !(status = check_redir_in(*tmp, all[0], all[1], i))))
        return ERROR;
    if (status == NOTHING_DONE && contain_only(all[1][*i], "&| \t")) {
        *all[0] = NULL_CMD;
        return ERROR;
    }
    if (status == NOTHING_DONE)
        append_str_array(&(*tmp)->args, strdup(all[1][*i]));
    return NOTHING_DONE;
}
