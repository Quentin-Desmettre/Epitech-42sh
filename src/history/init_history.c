/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** init_history
*/

#include "minishell.h"

int is_command_valid(char *command)
{
    if (!command)
        return 0;
    for (int i = 0; command[i]; i++) {
        if (command[i] == '!')
            return 0;
    }
    for (int i = 0; command[i]; i++) {
        if (command[i] != ' ' && command[i] != '\t')
            break;
        if (i == my_strlen(command) - 1)
            return 0;
    }
    return 1;
}

char *get_up(hist_t **history)
{
    hist_t *tmp = *history;

    if (history == NULL || *history == NULL)
        return NULL;
    if (tmp->select)
        return tmp->command;
    while (tmp->next && tmp->next->select == 0)
        tmp = tmp->next;
    tmp->select = 1;
    if (tmp->next == NULL)
        return tmp->command;
    tmp->next->select = 0;
    return tmp->command;
}

char *get_down(hist_t **history)
{
    hist_t *tmp = *history;

    if (history == NULL || *history == NULL)
        return NULL;
    while (tmp->next && tmp->select == 0)
        tmp = tmp->next;
    tmp->select = 0;
    if (tmp->next == NULL)
        return NULL;
    tmp->next->select = 1;
    return tmp->next->command;
}

void history_append(char *command, hist_t **history)
{
    hist_t *data;
    int fd = open(glob_history(NULL), O_RDWR | O_CREAT | O_APPEND, 0644);

    if (is_command_valid(command) == 0)
        return;
    data = malloc(sizeof(hist_t));
    data->command = my_strdup(command);
    if (!append_history_node(history, data)) {
        if (fd != -1)
            close(fd);
        return reset_history(history);
    }
    reset_history(history);
    if (fd == -1)
        return;
    dprint(fd, "%s\n", command);
    close(fd);
}

hist_t *init_history(void)
{
    hist_t *history = NULL;
    int fd_create = open(glob_history(NULL), O_RDWR | O_CREAT, 0644);
    FILE *fd = fopen(glob_history(NULL), "r+");
    char *buffer = NULL;
    size_t size = 0;
    hist_t *data;

    if (fd_create == -1)
        return NULL;
    close(fd_create);
    for (; getline(&buffer, &size, fd) > 0; free(buffer)) {
        data = malloc(sizeof(hist_t));
        memset(data, 0, sizeof(hist_t));
        data->command = my_strdup(buffer);
        append_history_node(&history, data);
        size = 0;
    }
    buffer ? free(buffer) : 0;
    fclose(fd);
    return history;
}
