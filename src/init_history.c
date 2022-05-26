/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** main.c
*/

#include <sys/stat.h>
#include "minishell.h"

histo_t *init_node(char *cmd)
{
    histo_t *ele = malloc(sizeof(histo_t));

    ele->command = cmd;
    ele->select = 0;
    ele->next = NULL;
    return (ele);
}

void push_node(histo_t *head, histo_t *data)
{
    for (int i = 0; head->next != NULL; i++)
        head = head->next;
    head->next = data;
}

long size_file(char *file)
{
    struct stat stats;

    if (stat(file, &stats) == 0)
        return (stats.st_size);
    return 0;
}

void give(char *file, histo_t *head)
{
    char *buff = NULL;
    char **map = NULL;
    int fd = open(file, O_RDONLY);
    long size = size_file(file);

    if (fd < 0 || size <= 0)
        return;
    buff = calloc(size + 1, sizeof(char));
    if (read(fd, buff, size) != size)
        return free(buff);
    map = my_str_to_word_array(buff, "\n");
    free(buff);
    for (int i = 0; map[i] != NULL; i++)
        push_node(head, init_node(map[i]));
    free(map);
    close(fd);
}
