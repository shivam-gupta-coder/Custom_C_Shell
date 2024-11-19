

#include "baapheader.h"
void proclore(char *pid)
{
    char path[256], buffer[1024];
    FILE *file;
    snprintf(path, sizeof(path), "/proc/%s/status", pid);

    file = fopen(path, "r");
    if (file == NULL)
    {
        perror("Error opening status file");
        return;
    }
    char process_state[32] = "";
    char process_group[32] = "";
    char virtual_memory[32] = "";
    char executable_path[256] = "";

    while (fgets(buffer, sizeof(buffer), file))
    {
        if (strncmp(buffer, "State:", 6) == 0)
        {
            sscanf(buffer, "State: %s", process_state);
        }
        else if (strncmp(buffer, "VmSize:", 7) == 0)
        {
            sscanf(buffer, "VmSize: %s", virtual_memory);
        }
        else if (strncmp(buffer, "Tgid:", 5) == 0)
        {
            sscanf(buffer, "Tgid: %s", process_group);
        }
    }
    fclose(file);

    // Construct the path to /proc/[pid]/exe
    snprintf(path, sizeof(path), "/proc/%s/exe", pid);
    ssize_t len = readlink(path, executable_path, sizeof(executable_path) - 1);
    if (len != -1)
    {
        executable_path[len] = '\0';
    }
    else
    {
        strcpy(executable_path, "N/A");
    }

    // Print the process information
    printf("pid : %s\n", pid);
    printf("process status : %s\n", process_state);
    printf("Process Group : %s\n", process_group);
    printf("Virtual memory : %s\n", virtual_memory);
    printf("executable path : %s\n", executable_path);
}
void print_in_color(const char *color, const char *text)
{
    printf("%s%s%s\n", color, text, "\033[0m"); // Reset color after printing
}