#include "baapheader.h"
void ping_command(pid_t pid, int signal_number)
{
    if (kill(pid, 0) == -1)
    {
        perror("No such process found");
        return;
    }
    int mod_signal = signal_number % 32;

    if (kill(pid, mod_signal) == -1)
    {
        perror("Failed to send signal");
        return;
    }
    status_of_process();
    printf("Sent signal %d to process with pid %d\n", mod_signal, pid);
}