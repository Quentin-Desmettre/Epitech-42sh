/*
** EPITECH PROJECT, 2021
** B-CPE-200-LIL-2-1-lemin-quentin.desmettre
** File description:
** str_is_num.c
*/

#include "libmy.h"

int str_is_num(char const *str)
{
    int i = 0;

    while (str[i]) {
        if (str[i] > '9' || str[i] < '0')
            return 0;
        i++;
    }
    return 1;
}
