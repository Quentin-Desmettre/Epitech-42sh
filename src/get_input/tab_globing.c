/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** shell_print.c
*/

#include "minishell.h"
#include <glob.h>
#include <sys/types.h>
#include <sys/stat.h>

char **one_word(char **first)
{
    int size = my_str_array_len(first);
    char **result = malloc(sizeof(char *) * (size + 1));
    int j = 0;

    result[0] = NULL;
    for (int i = 0; i < size; i++) {
        if (!is_in_arr(result, first[i])) {
            result[j] = first[i];
            j++;
            result[j] = NULL;
        } else
            free(first[i]);
    }
    return result++;
}

int curent_file_tab(char *tmp, glob_t *glob_buf)
{
    struct stat sb;
    int start;
    glob_buf->gl_offs = 0;
    glob(tmp, GLOB_DOOFFS, NULL, glob_buf);
    for (int i = 0; glob_buf->gl_pathv[i]; i++) {
        start = stat(glob_buf->gl_pathv[i], &sb);
        if (!start && S_ISDIR(sb.st_mode)) {
            glob_buf->gl_pathv[i] = strcat(realloc(glob_buf->gl_pathv[i],
            strlen(glob_buf->gl_pathv[i]) + 2), "/");
            continue;
        }
        if (!access(glob_buf->gl_pathv[i], X_OK)) {
            glob_buf->gl_pathv[i] = strcat(realloc(glob_buf->gl_pathv[i],
            strlen(glob_buf->gl_pathv[i]) + 2), "*");
            continue;
        }
    }
    return glob_buf->gl_pathc;
}

char **do_glob(char *tmp)
{
    glob_t glob_buf;
    struct stat sb;
    char **path = get_paths();
    char **commands;
    int start = curent_file_tab(tmp, &glob_buf);
    if (strlen(tmp) != 1 && !contain(tmp, '/'))
        for (int i = 0; path[i]; i++) {
            chdir(path[i]);
            glob(tmp, GLOB_DOOFFS | GLOB_APPEND, NULL, &glob_buf);
        }
    for (int i = start; glob_buf.gl_pathv[i]; i++) {
        start = stat(glob_buf.gl_pathv[i], &sb);
        if (!start && S_ISDIR(sb.st_mode)) {
            remove_string_arr(glob_buf.gl_pathv, i--);
            glob_buf.gl_pathc--;
        }
    }
    commands = one_word(glob_buf.gl_pathv);
    my_free("Pp", path, glob_buf.gl_pathv);
    return commands;
}

char *get_tmp_char(input_t *input)
{
    char *tmp = malloc(sizeof(char) * (input->buf_size + 2));
    int wrd;

    for (wrd = input->key_pos; wrd > 0; wrd--)
        if (contain("\t| ;&<>", input->buffer[wrd])) {
            wrd++;
            break;
        }
    strncpy(tmp, input->buffer + wrd, input->buf_size - wrd);
    tmp[input->key_pos - wrd] = '\0';
    return tmp;
}

void globing_all_file(input_t *input, char const *prompt,
hist_t **history)
{
    char *tmp = get_tmp_char(input);
    char *wd = malloc(sizeof(char) * 4096);
    char **commands;

    if (contain(tmp, '*'))
        return my_free("pp", wd, tmp);
    strcat(tmp, "*");
    getcwd(wd, 4096);
    commands = do_glob(tmp);
    chdir(wd);
    if (my_str_array_len(commands) > 1) {
        set_print_tab(commands, input, prompt, tmp);
        rest_replace_buffer(input, commands, prompt, history);
    } else if (my_str_array_len(commands) == 1)
        replace_buffer(input,
        (char *[2]){commands[0], (char *)prompt}, 1, history);
    my_free("ppP", wd, tmp, commands);
}
