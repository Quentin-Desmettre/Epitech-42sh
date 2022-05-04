/*
** EPITECH PROJECT, 2021
** my_put_nbr
** File description:
** Function that displays the number given as a parameter
*/

#include "libmy.h"

char *nbr_to_str(unsigned nb)
{
    int size = nb ? 1 : 2;
    int cp = nb;
    char *str;

    for (; cp; size++)
        cp /= 10;
    str = malloc(sizeof(char) * size);
    str[size - 1] = 0;
    if (!nb) {
        str[0] = '0';
        return str;
    }
    while (nb) {
        str[size - 2] = nb % 10 + 48;
        nb /= 10;
        size--;
    }
    return str;
}

static void do_recursion_part(int nb)
{
    if (nb >= 0 && nb <= 9) {
        my_putchar(nb + 48);
    } else {
        my_put_nbr(nb / 10);
        my_putchar(nb % 10 + 48);
    }
}

int my_put_nbr(int nb)
{
    if (nb < 0) {
        my_putchar('-');
        nb *= -1;
    }
    if (nb == -2147483648) {
        write(1, "-2147483648", 11);
        return (0);
    }
    do_recursion_part(nb);
    return (0);
}
