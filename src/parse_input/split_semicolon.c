/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** split_semicolon.c
*/

#include "minishell.h"

static list_t *split_separators(char **words)
{
    list_t *commands = NULL;
    char **tmp = calloc(1, sizeof(char *));
    int status;

    for (int i = 0; words[i]; i++) {
        if (!(status = check_and(words, i, &tmp, &commands)))
            return NULL;
        if (status == NOTHING_DONE &&
        !(status = check_or(words, i, &tmp, &commands)))
            return NULL;
        if (status == NOTHING_DONE)
            append_str_array(&tmp, strdup(words[i]));
        if (status == CONNECTION_DONE) {
            free_str_array(tmp, 0);
            tmp = calloc(1, sizeof(char *));
        }
    }
    if (tmp[0] && !create_link(tmp, NO_TYPE, &commands))
        return NULL;
    return commands;
}

static list_t *split_semicolonss(char **words)
{
    list_t *commands = NULL;
    char **tmp = calloc(1, sizeof(char *));

    for (int i = 0; words[i]; i++) {
        if (contain_only(words[i], "; \t") && tmp[0]) {
            append_node(&commands, tmp);
            tmp = calloc(1, sizeof(char *));
        }
        if (contain_only(words[i], "; \t"))
            continue;
        append_str_array(&tmp, strdup(words[i]));
    }
    if (tmp[0])
        append_node(&commands, tmp);
    return commands;
}

static void exec_command_list(list_t *commands, env_t *vars)
{
    list_t *begin = commands;
    command_link_t *link;

    do {
        link = commands->data;
        exec_commands(link->commands, vars, &link->commands);
        if (link->link_type == AND_TYPE && get_last_exit() != 0)
            break;
        if (link->link_type == OR_TYPE && get_last_exit() == 0)
            break;
        if (link->link_type == NO_TYPE)
            break;
        commands = commands->next;
    } while (commands != begin);
}

static list_t *split_separatorss(list_t *semicolons)
{
    list_t *all_command_links = NULL;
    list_t *begin = semicolons;
    list_t *separated;

    do {
        separated = split_separators(semicolons->data);
        if (!separated)
            return NULL;
        append_node(&all_command_links, separated);
        semicolons = semicolons->next;
    } while (begin != semicolons);
    return all_command_links;
}

void new_parse_input(char *input, env_t *vars)
{
    char **words = split_words(input, vars);
    list_t *first_split = words ? split_semicolonss(words) : NULL;
    list_t *second_split = words ? split_separatorss(first_split) : NULL;
    list_t *begin = second_split;

    if (!second_split)
        return;
    do {
        exec_command_list(second_split->data, vars);
        second_split = second_split->next;
    } while (second_split != begin);
    if (is_exit()) {
        write(2, "exit\n", 5);
        exit(get_exit_status());
    }
}
