/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** cd_get_infos.c
*/

#include "minishell.h"
#include "cd.h"

static void replace_tilde(char **dir, char const *home_dir, int len_home)
{
    if (!home_dir)
        return;
    if (!my_strncmp(*dir, home_dir, len_home))
        re_alloc(dir, replace(*dir, 0, len_home, "~"), 1);
}

int print_flags(int *flags, char const *home, int len_home, int o_fd)
{
    char *buf;

    if (!flags[0] && !flags[1] && !flags[2] && !flags[3])
        return 1;
    buf = get_current_dir();
    if (!buf)
        return 0;
    if (flags[0] && !flags[1] && !flags[2] && !flags[3])
        replace_tilde(&buf, home, len_home);
    if (flags[2])
        write(o_fd, "0\t", 2);
    dprint(o_fd, "%s\n", buf);
    free(buf);
    return 1;
}

char *get_current_dir(void)
{
    char *buf = malloc(sizeof(char) * 4096);

    if (!getcwd(buf, 4096)) {
        dprint(2, "%s: %s.\n", "Error: ", strerror(errno));
        free(buf);
        set_last_exit(1);
        return NULL;
    }
    return buf;
}

int get_cd_flags(char *str, int *flags)
{
    for (int i = 1; str[i]; i++) {
        if (!contain(CD_FLAGS, str[i]))
            return dprint(2, "Usage: cd [-plvn][-|<dir>].\n") ? 0 : 0;
        flags[index_of(str[i], CD_FLAGS)] = 1;
    }
    return 1;
}

char *get_dir(char const *dir, char **env, char const *home)
{
    (void)env;
    if (!dir)
        return my_strdup(home);
    else if (!my_strcmp(dir, "-"))
        return my_strdup(get_oldwd());
    else
        return my_strdup(dir);
}
