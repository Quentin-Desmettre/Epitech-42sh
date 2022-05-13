/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** split_semicolon.c
*/

#include "minishell.h"

#define AND_TYPE 0
#define OR_TYPE 1
#define NO_TYPE 2

typedef struct {
    int link_type;
    char **words;
} command_link_t;

void m_append_str_array(char ***array, char *new_str, int dup_str)
{
    int len = my_str_array_len(*array);
    char **new = realloc(*array, ((len + 1) * sizeof(char *)));

    new[len] = new_str;
    if (dup_str)
        new[len] = my_strdup(new_str);
    new[len + 1] = NULL;
    *array = new;
}

list_t *get_command_lists(char **words, char **err_mess)
{
    list_t *commands = NULL;
    command_t *tmp = create_command();

    for (int i = 0; words[i]; i++) {
        if (!my_strcmp(words[i], ">>")) {
            if (HAS_REDIR_OUT(tmp)) {
                *err_mess = AMBIG_OUT;
                return NULL;
            }
            tmp->redir_type += REDIR_OUT_APP;
            tmp->output_file = words[i + 1];
            if (!words[i + 1]) {
                *err_mess = NULL_CMD;
                return NULL;
            }
            i++;
        } else if (!my_strcmp(words[i], "<<")) {
            if (HAS_REDIR_IN(tmp)) {
                *err_mess = AMBIG_IN;
                return NULL;
            }
            tmp->redir_type += REDIR_IN2;
            tmp->input_file = words[i + 1];
            if (!words[i + 1]) {
                *err_mess = NULL_CMD;
                return NULL;
            }
            i++;
        } else if (!my_strcmp(words[i], ">")) {
            if (HAS_REDIR_OUT(tmp)) {
                *err_mess = AMBIG_OUT;
                return NULL;
            }
            tmp->redir_type += REDIR_OUT;
            tmp->output_file = words[i + 1];
            if (!words[i + 1]) {
                *err_mess = NULL_CMD;
                return NULL;
            }
                i++;
        } else if (!my_strcmp(words[i], "<")) {
            if (HAS_REDIR_IN(tmp)) {
                *err_mess = AMBIG_IN;
                return NULL;
            }
            tmp->redir_type += REDIR_IN;
            tmp->input_file = words[i + 1];
            if (!words[i + 1]) {
                *err_mess = NULL_CMD;
                return NULL;
            }
            i++;
        } else if (!my_strcmp(words[i], "|")) {
            if (HAS_REDIR_OUT(tmp)) {
                *err_mess = AMBIG_OUT;
                return NULL;
            }
            tmp->redir_type += REDIR_OUT;
            tmp->output_file = NULL;
            if (!words[i + 1] || !tmp->args[0] || !my_strcmp(words[i + 1], "|")) {
                *err_mess = NULL_CMD;
                return NULL;
            }
            append_node(&commands, tmp);
            tmp = create_command();
            tmp->redir_type += REDIR_IN;
        } else {
            append_str_array(&tmp->args, strdup(words[i]));
        }
    }
    if (tmp->args[0])
        append_node(&commands, tmp);
    else if (HAS_REDIR_OUT(tmp) || HAS_REDIR_IN(tmp)) {
        *err_mess = NULL_CMD;
        return NULL;
    }
    return commands;
}

command_link_t *create_command_link(int type, char **words)
{
    command_link_t *c = malloc(sizeof(command_link_t));

    c->link_type = type;
    c->words = words;
    return c;
}

char **split_input(char const *input)
{
    return my_str_to_word_array(input, " \t\n");
}

list_t *new_split_semicolons(char const *input)
{
    char **words = split_input(input);
    list_t *commands = NULL;
    char **tmp = calloc(1, sizeof(char *));

    for (int i = 0; words[i]; i++) {
        if (contain_only(words[i], "; \t\n")) {
            append_node(&commands, tmp);
            tmp = calloc(1, sizeof(char *));
            continue;
        }
        append_str_array(&tmp, strdup(words[i]));
    }
    if (tmp[0])
        append_node(&commands, tmp);
    return commands;
}

list_t *split_separators(char **words)
{
    list_t *commands = NULL;
    char **tmp = calloc(1, sizeof(char *));

    for (int i = 0; words[i]; i++) {
        if (!strcmp(words[i], "&&")) {
            append_node(&commands, create_command_link(AND_TYPE, tmp));
            tmp = calloc(1, sizeof(char *));
            continue;
        }
        if (!strcmp(words[i], "||")) {
            append_node(&commands, create_command_link(OR_TYPE, tmp));
            tmp = calloc(1, sizeof(char *));
            continue;
        }
        append_str_array(&tmp, strdup(words[i]));
    }
    if (tmp[0])
        append_node(&commands, create_command_link(NO_TYPE, tmp));
    return commands;
}

void exec_separators(list_t *separators, env_t *vars)
{
    list_t *cmds;
    list_t *begin = separators;
    command_link_t *link;

    if (!separators)
        return;
    do {
        link = separators->data;
        cmds = get_command_lists(link->words, NULL);
        exec_commands(cmds, vars, &separators);
        if (link->link_type == AND_TYPE && get_last_exit() != 0)
            break;
        if (link->link_type == OR_TYPE && get_last_exit() == 0)
            break;
        if (link->link_type == NO_TYPE)
            break;
        separators = separators->next;
    } while (separators != begin);
}

list_t *get_all_links(list_t *semicolons)
{
    list_t *begin = semicolons;
    list_t *links = NULL;
    list_t *tmp;

    if (!semicolons)
        return NULL;
    do {
        tmp = split_separators(semicolons->data);
        if (!tmp)
            return NULL;
        append_node(&links, tmp);
        semicolons = semicolons->next;
    } while (begin != semicolons);
    return links;
}

void new_parse_input(char const *input, env_t *vars)
{
    list_t *all_links = get_all_links(new_split_semicolons(input));
    list_t *begin = all_links;

    if (!all_links) {
        set_last_exit(1);
        return;
    }
    do {
        exec_separators(all_links->data, vars);
        all_links = all_links->next;
    } while (all_links != begin);
}

list_t *split_semicolons(char const *input)
{
    char **words = my_str_to_word_array(input, ";");
    list_t *list = NULL;

    for (int i = 0; words[i]; i++)
        append_node(&list, words[i]);
    free(words);
    return list;
}
