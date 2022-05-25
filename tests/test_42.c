/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** test_42
*/

#include <criterion/criterion.h>
#include <stdio.h>

Test(test_isnbope1, test_nb_1)
{
    cr_assert_eq(error_base("12+34+5(56)", "0123456789", "()+-*%/"), 0);
    cr_assert_eq(error_base("12+34+5(5A)", "0123456789", "()+-*%/"), 1);
    cr_assert_eq(error_base("12+34+5(5)", "01234567899", "()+-*%/"), 1);
    cr_assert_eq(error_base("12+34+5(5)", "0123456789", "()+-9%/"), 1);
    cr_assert_eq(error_base("12+34+5(5)", "0123456789", "()+--%/"), 1);
}