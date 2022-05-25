/*
** EPITECH PROJECT, 2022
** CHECK_EXEC_MANIP_C
** File description:
** check_exec_manip.c
*/

#include "minishell.h"

int check_redir_out(command_t *tmp, char **err_mess, char **words, int *i)
{
    int is_db = !my_strcmp(words[*i], ">>");
    int is_sg = !my_strcmp(words[*i], ">");

    if (is_db || is_sg) {
        if (HAS_REDIR_OUT(tmp)) {
            *err_mess = AMBIG_OUT;
            return ERROR;
        }
        tmp->redir_type += is_db ? REDIR_OUT_APP : REDIR_OUT;
        tmp->output_file = words[*i + 1];
        if (!words[*i + 1]) {
            *err_mess = NULL_CMD;
            return ERROR;
        }
        (*i)++;
        return CONNECTION_DONE;
    }
    return NOTHING_DONE;
}

int check_redir_in(command_t *tmp, char **err_mess, char **words, int *i)
{
    int is_db = !my_strcmp(words[*i], "<<");
    int is_sg = !my_strcmp(words[*i], "<");

    if (is_db || is_sg) {
        if (HAS_REDIR_IN(tmp)) {
            *err_mess = AMBIG_IN;
            return ERROR;
        }
        tmp->redir_type += is_db ? REDIR_IN2 : REDIR_IN;
        tmp->input_file = words[*i + 1];
        if (!words[*i + 1]) {
            *err_mess = NULL_CMD;
            return ERROR;
        }
        (*i)++;
        return CONNECTION_DONE;
    }
    return NOTHING_DONE;
}

int check_pipe(command_t **tmp, char **all[2], int *i, list_t **commands)
{
    char **err_mess = all[0];
    char **words = all[1];

    if (!my_strcmp(words[*i], "|")) {
        if (HAS_REDIR_OUT((*tmp))) {
            *err_mess = AMBIG_OUT;
            return ERROR;
        }
        (*tmp)->redir_type += REDIR_OUT;
        (*tmp)->output_file = ERROR;
        if (!words[*i + 1] || !(*tmp)->args[0] ||
        !my_strcmp(words[*i + 1], "|"))
            return (*err_mess = NULL_CMD) ? ERROR : ERROR;
        append_node(commands, (*tmp));
        *tmp = create_command();
        (*tmp)->redir_type += REDIR_IN;
        return CONNECTION_DONE;
    }
    return NOTHING_DONE;
}

int check_and(char **words, int i, char ***tmp, list_t **commands)
{
    if (!strcmp(words[i], "&&")) {
        if (!is_prev_valid(words, i) || !is_next_valid(words, i)) {
            dprint(2, "Invalid null command.\n");
            return 0;
        }
        if (!create_link(*tmp, AND_TYPE, commands))
            return 0;
        *tmp = calloc(1, sizeof(char *));
        return CONNECTION_DONE;
    }
    return NOTHING_DONE;
}

int check_or(char **words, int i, char ***tmp, list_t **commands)
{
    if (!strcmp(words[i], "||")) {
        if (!is_prev_valid(words, i) || !is_next_valid(words, i)) {
            dprint(2, "Invalid null command.\n");
            return 0;
        }
        if (!create_link(*tmp, OR_TYPE, commands))
            return 0;
        tmp = calloc(1, sizeof(char *));
        return CONNECTION_DONE;
    }
    return NOTHING_DONE;
}
