/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** main.c
*/

#include "../include/minishell.h"
#include <sys/stat.h>

char **str_to_word_array_my(char *buff)
{
    int size = count_nbr(buff);
    int a = 0;
    int j = 0;
    char **map = malloc(sizeof(char *) * (size + 1));

    (map == NULL) ? (exit(84)) : (0);
    for (int i = 0; i != size; i++) {
        map[i] = malloc(sizeof(char) * (lenght_line(buff, i)));
        (map[i] == NULL) ? (exit(84)) : (0);
    }
    for (int i = 0; buff[i] != '\0'; i++) {
        map[j][a] = buff[i];
        a++;
        (buff[i] == '\n') ? (map[j][a] = '\0', j++, a = 0) : 0;
    }
    map[j] = 0;
    return map;
}

char *cp(char *array)
{
    int j = 0;
    int size = lenght_line(array, 0);
    char *new_map = malloc(sizeof(char) * size);
    if (new_map == NULL)
        exit(84);
    for (; array[j] != '\0'; j++)
        new_map[j] = array[j];
    new_map[j] = '\0';
    return new_map;
}

histo_t *init_node(char *cmd)
{
    histo_t *ele = malloc(sizeof(histo_t));
    ele->command = cmd;
    ele->select = 0;
    ele->next = NULL;
    return ele;
}

void push_node(histo_t *tete, histo_t *boulle)
{
    for (int i = 0; tete->next != NULL; i++)
        tete = tete->next;
    tete->next = boulle;
}

int size_file(char *file)
{
    struct stat stats;
    if (stat(file, &stats) == 0)
        return stats.st_size;
    else
        exit(84);
}

void give(char *file, histo_t *head)
{
    int lu;
    char buff[size_file(file)];
    char **map = NULL;
    int fd = open(file, O_RDONLY);

    if (fd < 0)
        return;
    lu = read(fd, buff, size_file(file));
    if (lu < 0)
        return;
    buff[lu] = '\0';
    map = str_to_word_array_my(buff);
    for (int i = 0; map[i] != NULL; i++) {
        push_node(head, init_node(cp(map[i])));
        free(map[i]);
    }
    free(map);
}
