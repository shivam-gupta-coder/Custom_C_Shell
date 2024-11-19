#include "baapheader.h"

void fg_command(pid_t pid)
{
    delete_process(pid);
    if (kill(pid, 0) == -1)
    {
        perror("No such process found");
        return;
    }
    kill(pid, SIGCONT);
    fg_pid = pid;
    tcsetpgrp(STDIN_FILENO, getpgid(pid));
    int status;
    while (waitpid(pid, &status, WUNTRACED) == -1 && errno == EINTR)
    {
        // do nothing
    }
    tcsetpgrp(STDIN_FILENO, getpgid(getpid()));
    status_of_process();

    if (WIFSTOPPED(status))
    {
        printf("Process [%d] stopped\n", pid);
    }
    else if (WIFEXITED(status))
    {
        printf("Process [%d] exited with status %d\n", pid, WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status))
    {
        printf("Process [%d] killed by signal %d\n", pid, WTERMSIG(status));
    }

}
void bg_command(pid_t pid)
{
    if (kill(pid, 0) == -1)
    {
        perror("No such process found");
        return;
    }
    if (kill(pid, SIGCONT) == -1)
    {
        perror("Failed to continue process");
        return;
    }
    status_of_process();
    printf("Process [%d] resumed in background\n", pid);
}