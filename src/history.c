/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** history of the 42
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/minishell.h"

int count_line(char *buff)
{
    int count = 0;

    for (int i = 0; buff[i] != '\0'; i++) {
        count++;
    }
    return count;
}

int count_nbr(char *buff)
{
    int count = 0;

    for (int i = 0; buff[i] != '\0'; i++) {
        if (buff[i] == '\n')
            count++;
    }
    return count;
}

int lenght_line(char *buffer, int i)
{
    int j = 0;
    int bn = 0;
    int count = 0;

    for (j; i != bn; j++)
        if (buffer[j] == '\n')
            bn++;
    for (j; buffer[j] != '\n'; j++)
        count++;
    return count + 2;
}

char **str_to_word_array(char *buff)
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

char *cp(char *array, int i)
{
    int j = 0;
    int size = lenght_line(array, 0);
    char *new_map = malloc(sizeof(char) * size);
    if (new_map == NULL)
        exit(84);
    for (j; array[j] != '\n'; j++)
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

void give(char *file, histo_t *head)
{
    int i = 0;
    int size = 0;
    char *nm = NULL;
    int lu = 0;
    char buff[30000];
    char **map = NULL;
    int fd = open(file, O_RDONLY);

    if (fd < 0)
        return;
    lu = read(fd, buff, 29999);
    buff[lu] = '\0';
    map = str_to_word_array(buff);
    for (int i = 0; map[i] != NULL; i++)
        size++;
    for (int i = 0; map[i] != NULL; i++)
        push_node(head, init_node(cp(map[i], 0)));
    for (int i = 0; map[i] != NULL; i++)
        free(map[i]);
    free(map);
}

void free_history_list(histo_t *head, histo_t *temp)
{
    while (head->next != NULL)
    {
        temp = head->next;
        free(head->command);
        free(head);
        head = temp;
    }
}

void history_append(char *command, histo_t *head)
{
    int fd = open("../history.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);

    if (fd < 0)
        return;
    push_node(head, init_node(cp(command, 0)));
    write(fd, command, count_line(command));
    write(fd, "\n", 1);
    close(fd);
}

char *is_down(histo_t *head)
{
    while (head->next != NULL) {
        if (head->select == 1) {
            head->select = 0;
            head->next->select = 1;
            return head->next->command;
        }
        head = head->next;
    }
    return head->command;
}

char *is_up(histo_t *head)
{
    while (head->next != NULL) {
        if (head->next->select == 1) {
            head->next->select = 0;
            head->select = 1;
            return head->command;
        }
        head = head->next;
    }
    head->select = 1;
    return head->command;
}

void print_ll(histo_t *head)
{
    while (head != NULL)
    {
        printf("%s\n", head->command);
        head = head->next;
    }
}

int main(void)
{
    histo_t *head = init_node(cp("fd\n", 0));
    histo_t *temp;
    give("../history.txt", head);
    if (head->next != NULL) {
        temp = head->next;
        free(head->command);
        free(head);
        head = temp;
    }
    print_ll(head);
    history_append("ls -la", head);
    print_ll(head);
    printf("\n\n");
    printf("%s\n", is_up(head));
    printf("%s\n", is_up(head));
    printf("%s\n", is_down(head));
    printf("%s\n", is_down(head));
    printf("%s\n", is_down(head));
    printf("%s\n", is_down(head));
    printf("%s\n", is_down(head));
    free_history_list(head, temp);
}