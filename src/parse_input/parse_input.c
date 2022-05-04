/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** parse_input.c
*/

#include "minishell.h"

int free_exit(list_t **commands, command_t *tmp, char *cmd, int status)
{
    free(cmd);
    free_command(tmp);
    destroy_list(commands, free_command);
    set_last_exit(1);
    return status;
}

char *get_error_message(char *cmd, char const *command, int i, command_t *tmp)
{
    if (!cmd[0])
        return NULL_CMD;
    if (command[i] == '>' && HAS_REDIR_OUT(tmp))
        return AMBIG_OUT;
    if (command[i] == '<' && HAS_REDIR_IN(tmp))
        return AMBIG_IN;
    return NULL;
}

static int check_last_arg(char *cmd,
command_t *tmp, char **err_mess, list_t **list)
{
    if (cmd[0]) {
        tmp->args = my_str_to_word_array(cmd, " \t");
        if (!tmp->args[0]) {
            *err_mess = NULL_CMD;
            return free_exit(list, tmp, cmd, ERROR);
        }
        free(cmd);
        append_node(list, tmp);
    } else
        free(cmd);
    return NOTHING;
}

list_t *get_command_list(char const *command, char **err_mess)
{
    list_t *list = NULL;
    command_t *tmp = create_command();
    char *cmd = my_strdup("");
    int status;
    void *strings[3] = {command, &cmd, err_mess};

    for (int i = 0; command[i]; i++) {
        status = check_redirections(strings, &i, &list, &tmp);
        if (status == ERROR)
            return NULL;
        if (status == NOTHING)
            append_char(&cmd, command[i], 1);
    }
    if (check_last_arg(cmd, tmp, err_mess, &list) == ERROR)
        return NULL;
    return list;
}
