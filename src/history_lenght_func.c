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

    for (; i != bn; j++)
        if (buffer[j] == '\n')
            bn++;
    for (; buffer[j] != '\0'; j++)
        count++;
    return count + 2;
}
