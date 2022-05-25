/*
** EPITECH PROJECT, 2022
** GENERATE_COMMAND_C
** File description:
** generate_command.c
*/

#include "minishell.h"

static list_t *get_command_lists(char **words, char **err_mess)
{
    list_t *commands = NULL;
    command_t *tmp = create_command();

    for (int i = 0; words[i]; i++) {
        if (!check_everything(&tmp,
        (char **[2]){err_mess, words}, &i, &commands))
            return NULL;
    }
    if (tmp->args[0])
        append_node(&commands, tmp);
    else if (HAS_REDIR_OUT(tmp) || HAS_REDIR_IN(tmp)) {
        *err_mess = NULL_CMD;
        return NULL;
    }
    return commands;
}

static command_link_t *create_command_link(int type, char **words)
{
    command_link_t *c = malloc(sizeof(command_link_t));
    char *err_mess = NULL;

    c->link_type = type;
    c->commands = get_command_lists(words, &err_mess);
    if (!c->commands || err_mess) {
        dprint(2, "%s\n", err_mess ? err_mess : "Invalid null command.");
        set_last_exit(1);
        free(c);
        if (!isatty(0))
            exit(0);
        return NULL;
    }
    return c;
}

int is_prev_valid(char **words, int current)
{
    if (!current)
        return 0;
    if (contain_only(words[current - 1], "|&; \t"))
        return 0;
    return 1;
}

int is_next_valid(char **words, int current)
{
    if (!words[current + 1])
        return 0;
    if (contain_only(words[current + 1], "|&; \t"))
        return 0;
    return 1;
}

int create_link(char **tmp, int type, list_t **commands)
{
    command_link_t *link = create_command_link(type, tmp);

    if (!link)
        return 0;
    append_node(commands, link);
    return 1;
}
