/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** command.c
*/

#include "minishell.h"

command_t *create_command(void)
{
    command_t *cmd = malloc(sizeof(command_t));

    cmd->args = calloc(1, sizeof(char *));
    cmd->input_fd = -1;
    cmd->input_file = NULL;
    cmd->output_fd = -1;
    cmd->output_file = NULL;
    cmd->redir_type = REDIR_NONE;
    return cmd;
}

void free_command(void *commnd)
{
    command_t *cmd = commnd;

    if (!cmd)
        return;
    free_str_array(cmd->args, 1);
    free(cmd->input_file);
    free(cmd->output_file);
    free(cmd);
}
