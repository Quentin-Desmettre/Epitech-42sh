/*
** EPITECH PROJECT, 2021
** mr_revstr
** File description:
** reverse a string
*/

#include "libmy.h"

char *my_revstr(char *str)
{
    int length = my_strlen(str);

    for (int index = 0; index < length / 2; ++index) {
        my_char_swap(str + index, str + length - index - 1);
    }
    return str;
}
