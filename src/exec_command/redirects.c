/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** redirects.c
*/

#include "minishell.h"

void check_exit_status_fd(int status, int *has_crashed)
{
    if (WIFEXITED(status)) {
        if (!(*has_crashed))
            set_last_exit(WEXITSTATUS(status));
        return;
    }
    if (WIFSIGNALED(status)) {
        if (WTERMSIG(status) == SIGINT) {
            set_last_exit(write(2, "\n", 1) ? 1 : 1);
            return;
        }
        set_last_exit(WTERMSIG(status) + 128);
        if (*has_crashed && WCOREDUMP(status))
            write(2, "  (core dumped)", 15);
        if (WTERMSIG(status) == SIGFPE && !(*has_crashed))
            dprint(2, "Floating exception%s", CRASH_STR(status));
        if (WTERMSIG(status) != SIGFPE && !(*has_crashed))
            dprint(2, "%s%s", strsignal(WTERMSIG(status)), CRASH_STR(status));
        *has_crashed = 1;
    }
}

int get_output_fd(command_t *cur, int fds[2], int *is_pipe, list_t *commands)
{
    if (IS_REDIR_OUT(cur->redir_type) && cur->output_file)
        fds[1] = open(cur->output_file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (IS_REDIR_OUT(cur->redir_type) && !cur->output_file) {
        if (pipe(fds) < 0)
            return dprint(2, "pipe: %s.\n", strerror(errno)) ? ERROR : ERROR;
        *is_pipe = 1;
        COMMAND((commands)->next->data)->input_fd = fds[0];
        fds[0] = 0;
    }
    if (IS_REDIR_OUT_APP(cur->redir_type))
        fds[1] = open(cur->output_file, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fds[1] < 0)
        return dprint(2, "%s: %s.\n",
        cur->output_file, strerror(errno)) == -60;
    return NOTHING;
}

int get_input_fd(command_t *cur, int fds[2])
{
    if (IS_REDIR_IN(cur->redir_type) && cur->input_file)
        fds[0] = open(cur->input_file, O_RDONLY);
    if (IS_REDIR_IN(cur->redir_type) && !cur->input_file)
        fds[0] = cur->input_fd;
    if (IS_REDIR_IN2(cur->redir_type)) {
        fds[0] = get_heredoc(cur->input_file);
        if (fds[0] < 0)
            return MAJOR_ERROR;
    }
    if (fds[0] < 0) {
        dprint(2, "%s: %s.\n", cur->input_file, strerror(errno));
        return ERROR;
    }
    return NOTHING;
}

static void launch_process(command_t *cur,
void *params[6], char const *command)
{
    int *fds = params[0];
    int *pids = params[1];
    char ***env = &((env_t *)params[3])->env;
    list_t **commands = params[5];
    char **args = dup_word_array(cur->args);

    signal(SIGINT, SIG_DFL);
    if (!redirect_pipe(fds) || not_global(&args)) {
        free(pids);
        exit(1);
    }
    if ((*commands)->next == (*commands) &&
    get_final_fd() >= 0 && !HAS_REDIR_OUT(cur)) {
        dup2(get_final_fd(), 1);
        close(get_final_fd());
    }
    free(pids);
    if (command)
        exec_args(command, args, *env);
    exit(1);
}

int exec_command(command_t *cur, void *params[6], int is_pipe, int i)
{
    int *pids = params[1];
    int *cannot_find = params[2];
    char const *command;

    if (is_builtin(cur->args[0]))
        pids[i] = exec_builtin_fd(cur->args, cur, params, is_pipe);
    else {
        command = find_command(cur->args[0]);
        pids[i] = fork();
        *cannot_find = (command && (pids[i] >= 0)) ? *cannot_find : 1;
        if (pids[i] < 0) {
            free((char *)command);
            return dprint(2, "fork: %s.\n", strerror(errno)) ? 4 : 4;
        }
        if (!pids[i])
            launch_process(cur, params, command);
        free((char *)command);
    }
    return NOTHING;
}
