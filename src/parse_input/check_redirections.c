/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** check_redirections.c
*/

#include "minishell.h"

static int check_single_redir(void *strings[3],
int *i, list_t **list, command_t *tmp)
{
    char const *command = strings[0];
    char *cmd = *(char **)(strings[1]);
    char **err_mess = strings[2];
    char **next_word;

    if (contain("><", command[*i])) {
        *err_mess = get_error_message(cmd, command, *i, tmp);
        if (*err_mess)
            return free_exit(list, tmp, cmd, ERROR);

        tmp->redir_type += (command[*i] == '>' ? REDIR_OUT : REDIR_IN);
        next_word =
        (command[*i] == '>' ? &tmp->output_file : &tmp->input_file);
        *next_word = get_next_word(command, i, err_mess);

        if (!tmp->output_file && !tmp->input_file)
            return free_exit(list, tmp, cmd, ERROR);
        return CONTINUE;
    }
    return NOTHING;
}

static int check_double_redir(void *strings[3],
int *i, list_t **list, command_t *tmp)
{
    char const *command = strings[0];
    char *cmd = *(char **)(strings[1]);
    char **err_mess = strings[2];
    char **next_word;

    if (IS_DB_REDIR(command, *i)) {
        *err_mess = get_error_message(cmd, command, *i, tmp);
        if (*err_mess)
            return free_exit(list, tmp, cmd, ERROR);
        tmp->redir_type += (command[*i] == '>' ? REDIR_OUT_APP : REDIR_IN2);
        (*i)++;
        next_word =
        ((command[*i] == '>') ? &tmp->output_file : &tmp->input_file);
        *next_word = get_next_word(command, i, err_mess);
        if (!tmp->output_file && !tmp->input_file)
            return free_exit(list, tmp, cmd, ERROR);
        return CONTINUE;
    }
    return NOTHING;
}

static char **get_pipe_args(void *strings[3], command_t *tmp, int i)
{
    char const *command = strings[0];
    char *cmd = *(char **)(strings[1]);
    char **err_mess = strings[2];
    char **args;

    if (IS_REDIR_OUT(tmp->redir_type) || IS_REDIR_OUT_APP(tmp->redir_type)) {
        *err_mess = AMBIG_OUT;
        return NULL;
    }
    if (!cmd[0] || !can_get_next_word(command, i, err_mess)) {
        *err_mess = NULL_CMD;
        return NULL;
    }
    args = my_str_to_word_array(cmd, " \t");
    if (!args[0]) {
        *err_mess = NULL_CMD;
        free_str_array(args, 1);
        return NULL;
    }
    return args;
}

static int check_pipe(void *strings[3], int i, list_t **list, command_t **tmp)
{
    char const *command = strings[0];
    char **cmd = strings[1];

    if (command[i] == '|') {
        (*tmp)->args = get_pipe_args(strings, *tmp, i);
        if (!(*tmp)->args)
            return free_exit(list, *tmp, *cmd, ERROR);
        (*tmp)->redir_type += REDIR_OUT;
        (*tmp)->output_file = NULL;
        append_node(list, *tmp);
        *tmp = create_command();
        (*tmp)->redir_type += REDIR_IN;
        free(*cmd);
        *cmd = my_strdup("");
        return CONTINUE;
    }
    return NOTHING;
}

int check_redirections(void *strings[3],
int *i, list_t **list, command_t **tmp)
{
    int status;

    status = check_pipe(strings, *i, list, tmp);
    if (status != NOTHING)
        return status;

    status = check_double_redir(strings, i, list, *tmp);
    if (status != NOTHING)
        return status;

    status = check_single_redir(strings, i, list, *tmp);
    if (status != NOTHING)
        return status;

    return NOTHING;
}
