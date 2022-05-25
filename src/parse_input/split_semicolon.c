/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** split_semicolon.c
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

static int is_prev_valid(char **words, int current)
{
    if (!current)
        return 0;
    if (contain_only(words[current - 1], "|&; \t"))
        return 0;
    return 1;
}

static int is_next_valid(char **words, int current)
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

void find_field_aliases(char **str, list_t *info)
{
    list_t *save = info;
    replace_t *to_cmp = NULL;

    if (!*str || !save)
        return;
    do {
        to_cmp = save->data;
        if (my_strcmp(*str, to_cmp->name) == 0) {
            *str = to_cmp->value;
            break;
        }
        save = save->next;
    } while (save != info);
}

void replace_aliases_in_word_parse(char **word_parse, list_t *vars)
{
    if (!vars)
        return;
    find_field_aliases(&word_parse[0], vars);
    for (int i = 1; word_parse[i]; i++) {
        if (contain(word_parse[i], ';')) {
            find_field_aliases(&word_parse[i + 1], vars);
        }
    }
}

void delete_all_back_slash(char **str)
{
    int index;
    char *tmp = NULL;

    for (int i = 0; str[i]; i++) {
        while (contain(str[i], '\\')) {
            index = index_of('\\', str[i]);
            tmp = replace(str[i], index, 1, "");
            free(str[i]);
            str[i] = tmp;
        }
    }
}

char const *noa_better_get_field(char **env, char const *field, char spec)
{
    int f_len = my_strlen(field);

    for (int i = 0; env[i]; i++)
        if (my_strncmp(env[i], field, f_len) == 0 && env[i][f_len] == spec)
            return env[i] + f_len + 1;
    return NULL;
}

void get_variable(char **str, const char *source, int *len_var, int index_var)
{
    for (int i = index_var; source[i]; i++, (*len_var)++)
        if (source[i] == ' ' ||
        (i > 0 && source[i - 1] == ')') ||
        source [i] == '$' || source[i] == '\0')
            break;
    (*str) = malloc(sizeof(char) * (*len_var + 1));
    for (int i = 0; i < (*len_var) && source[index_var + i]; i++)
        (*str)[i] = source[index_var + i];
    (*str)[*len_var] = '\0';
}

void loop_to_replace(char **str, int *next_index, char **env, char spec)
{
    char *tmp = NULL;
    char *str_to_cmp = NULL;
    int len_to_replace = 0;

    for (; (*str)[*next_index]; (*next_index)++)
        if (*next_index && (*str)[*next_index - 1] == '$')
            break;
    get_variable(&str_to_cmp, (*str), &len_to_replace, *next_index);
    tmp = (char *)noa_better_get_field(env, str_to_cmp, spec);
    if (tmp) {
        tmp = replace((*str), *next_index - 1, len_to_replace + 1, tmp);
        free((*str));
        (*str) = tmp;
    }
}

void set_loop_to_replace(char **str, char **env, char spec)
{
    int nbr_of_var = 0;
    int next_index = 0;

    for (int j = 0; (*str)[j]; j++)
        nbr_of_var += (*str)[j] == '$' ? 1 : 0;
    while (nbr_of_var) {
        loop_to_replace(str, &next_index, env, spec);
        nbr_of_var--;
    }

}

void replace_all_variable(char **env, char **str, char spec)
{
    for (int i = 0; str[i]; i++) {
        if (contain(str[i], '$')) {
            set_loop_to_replace(&str[i], env, spec);
        }
    }
}

char **split_words(char *input, env_t *vars)
{
    char **word_parse = NULL;
    char *str_separator = ";&|><";

    input = clear_str(input);
    input = add_separator(str_separator, input);
    word_parse = str_to_word_array(input, " ");
    delete_all_back_slash(word_parse);
    replace_aliases_in_word_parse(word_parse, vars->aliases->commands);
    replace_all_variable(vars->vars, word_parse, '\t');
    replace_all_variable(vars->env, word_parse, '=');
    return (word_parse);
}

void new_parse_input(char *input, env_t *vars)
{
    char **words = split_words(input, vars);
    list_t *first_split = split_semicolonss(words);
    list_t *second_split = split_separatorss(first_split);
    list_t *begin = second_split;

    if (!second_split)
        return;
    do {
        exec_command_list(second_split->data, vars);
        second_split = second_split->next;
    } while (second_split != begin);
    if (is_exit()) {
        write(2, "exit\n", 5);
        exit(0);
    }
}
