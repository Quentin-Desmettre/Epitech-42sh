/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** cd.c
*/

#include "minishell.h"

static int check_cd_args(char **args, int *flags, int *has_dir, char **dir)
{
    *has_dir = 0;
    for (int i = 1; args[i]; i++) {
        if ((args[i][0] != '-' || !my_strcmp(args[i], "-")) && args[i + 1])
            return dprint(2, "cd: Too many arguments.\n") ? 0 : 0;
        if (args[i][0] != '-' || !my_strcmp(args[i], "-")) {
            *has_dir = 1;
            *dir = args[i];
            continue;
        }
        if (!get_cd_flags(args[i], flags))
            return 0;
    }
    return 1;
}

static int update_cd_env(char ***env, char *old_dir)
{
    char *cur_dir = get_current_dir();

    if (old_dir)
        setenv_pipe((char *[4]){"", "OLDPWD", old_dir, NULL}, env, 1, 0);
    if (cur_dir)
        setenv_pipe((char *[4]){"", "PWD", cur_dir, NULL}, env, 1, 0);
    free(cur_dir);
    free(old_dir);
    return 1;
}

static void change_dir(char const *dir, char ***env, char *old_dir, int *flags)
{
    char const *home = get_field(*env, "HOME=");

    if (chdir(dir)) {
        free(old_dir);
        dprint(2, "%s: %s.\n", dir, strerror(errno));
        set_last_exit(1);
    } else if (update_cd_env(env, old_dir) && print_flags(flags,
    home, my_strlen(home), flags[4]))
        set_last_exit(0);
    else
        set_last_exit(1);
}

static void replace_home(char **dir, char const *home_dir)
{
    int t = index_of('~', *dir);

    while (t >= 0) {
        re_alloc(dir, replace(*dir, t, 1, (char *)home_dir), 1);
        t = index_of('~', *dir);
    }
}

void cd_pipe(char **args, char ***env, int o_fd, int is_pipe)
{
    int flags[5] = {0, 0, 0, 0, o_fd};
    int has_dir;
    char *dir = NULL;
    char const *home = get_field(*env, "HOME=");
    char *old_dir = get_current_dir();

    if (!check_cd_args(args, flags, &has_dir, &dir)) {
        free(old_dir);
        set_last_exit(1);
        return;
    }
    if (is_pipe) {
        free(old_dir);
        return;
    }
    dir = get_dir(dir, *env, home);
    replace_home(&dir, home);
    change_dir(dir, env, old_dir, flags);
    free(dir);
}
