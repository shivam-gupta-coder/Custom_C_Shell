#include "baapheader.h"

char *logFilePath = "/home/shivamgupta/Desktop/proj1/comms.txt";
char commlog[LOG_NUM][MAX_STRING_LENGTH];
int commands = 0;
int idx = 0;
char currdir[PATH_MAX];
char prevdir[PATH_MAX];
char home[PATH_MAX];
int flag2sec = 0;
char command2sec[PATH_MAX];
int timeprocess = 0;
typedef struct
{
    bool only_dirs;
    bool only_files;
    bool execute_action;
} Flags;
char *trim_spaces(char *str)
{
    char *end;
    while (*str == ' ' || *str == '\t')
        str++;

    if (*str == 0)
        return str;
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t'))
        end--;
    *(end + 1) = '\0';

    return str;
}
void executelog(char *execcomm)
{
    char ip2[BUFFER_SIZE];
    strcpy(ip2, execcomm);
    char *save12;
    char *command12 = strtok_r(ip2, ";", &save12);
    while (command12 != NULL)
    {
        command12 = trim_spaces(command12);
        if (command12 == NULL || *command12 == '\0')
        {
            command12 = strtok_r(NULL, ";", &save12);
            continue;
        }
        while (strchr(command12, '&') != NULL)
        {
            char *bgcomm = strchr(command12, '&');
            *bgcomm = '\0';
            char *bgtrimcomm = trim_spaces(command12);
            if (bgtrimcomm == NULL || *bgtrimcomm == '\0')
            {
                break;
            }
            pid_t pid12 = fork();
            if (pid12 == 0)
            {
                printf("Background process started with PID %d\n", getpid());
                char *args[] = {"/bin/sh", "-c", bgtrimcomm, NULL};
                for (int i = 0; i < sizeof(args); i++)
                {
                    if (args[i] == NULL)
                    {
                        break;
                    }
                }
                int x = fork();
                if (x == 0)
                {
                    execvp(args[0], args);
                }
                else if (x > 0)
                {
                    wait(NULL);
                    printf("Background process %d finished with status %d\n", x, WEXITSTATUS(x));
                }
                else
                {
                    perror("fork");
                }
                perror("execvp");
                exit(1);
            }
            else if (pid12 > 0)
            {
                printf("[%d] %d\n", pid12, pid12);
            }
            else
            {
                perror("fork");
            }
            command12 = bgcomm + 1;
            command12 = trim_spaces(command12);
        }
        char *tokens12[BUFFER_SIZE];
        int tokenct2 = 0;
        char *save22;
        char *token12 = strtok_r(command12, " \t\n", &save22);
        while (token12 != NULL)
        {
            char *c = malloc(sizeof(char) * 40000);
            c = trim_spaces(token12);
            strcpy(token12, c);
            tokens12[tokenct2++] = token12;
            token12 = strtok_r(NULL, " \t\n", &save22);
        }
        if (tokenct2 > 0 && tokens12[0] != NULL)
        {
            if (strcmp(tokens12[0], "hop") == 0)
            {
                hop(tokens12, tokenct2);
            }
            else if (strcmp(tokens12[0], "reveal") == 0)
            {
                char *path11 = ".";
                int show_hidden11 = 0;
                int show_details11 = 0;
                for (int j11 = 1; j11 < tokenct2; j11++)
                {
                    if (tokens12[j11][0] == '-')
                    {
                        char *flag11 = tokens12[j11] + 1;
                        while (*flag11)
                        {
                            if (*flag11 == 'a')
                            {
                                show_hidden11 = 1;
                            }
                            else if (*flag11 == 'l')
                            {
                                show_details11 = 1;
                            }
                            flag11++;
                        }
                    }
                    else
                    {
                        path11 = tokens12[j11];
                    }
                }
                reveal(path11, show_hidden11, show_details11);
            }
            else if (strcmp(tokens12[0], "proclore") == 0)
            {
                if (tokenct2 == 1)
                {
                    char current_pid[32];
                    snprintf(current_pid, sizeof(current_pid), "%d", getpid());
                    proclore(current_pid);
                }
                else if (tokenct2 == 2)
                {
                    proclore(tokens12[1]);
                }
                else
                    printf("INVALID COMMAND!!\n");
            }
            else if (strcmp(tokens12[0], "seek") == 0)
            {
                int only_directories = 0;
                int only_files = 0;
                int execute_flag = 0;
                char *target_name = NULL;
                char *search_directory = ".";

                // Iterate over tokens to parse flags
                int token_index = 1; // Start after "seek"
                while (token_index < tokenct2 && tokens12[token_index][0] == '-')
                {
                    for (char *flag = tokens12[token_index] + 1; *flag; ++flag)
                    {
                        if (*flag == 'd')
                        {
                            only_directories = 1;
                        }
                        else if (*flag == 'f')
                        {
                            only_files = 1;
                        }
                        else if (*flag == 'e')
                        {
                            execute_flag = 1;
                        }
                        else
                        {
                            printf("Invalid flag: -%c\n", *flag);
                        }
                    }
                    token_index++;
                }

                if (only_directories && only_files)
                {
                    printf("Invalid flags! Cannot combine -d and -f.\n");
                }
                else
                {
                    if (token_index < tokenct2)
                    {
                        target_name = tokens12[token_index];
                        token_index++;
                    }
                    else
                    {
                        printf("Usage: seek [flags] target [directory]\n");
                    }
                    if (token_index < tokenct2)
                    {
                        search_directory = expand_path(tokens12[token_index]);
                    }

                    char results[1000][PATH_MAX];
                    int match_count = 0;
                    seek_in_directory(search_directory, target_name, only_directories, only_files, results, &match_count);

                    if (execute_flag)
                    {
                        if (match_count == 1)
                        {

                            struct stat path_stat;
                            stat(results[0], &path_stat);

                            if (S_ISDIR(path_stat.st_mode))
                            {
                                if (access(results[0], X_OK) != 0)
                                {
                                    printf("Missing permissions for task!\n");
                                }
                                else
                                {
                                    chdir(results[0]);
                                    printf("%s/\n", results[0]);
                                }
                            }
                            else if (S_ISREG(path_stat.st_mode))
                            {
                                if (access(results[0], R_OK) != 0)
                                {
                                    printf("Missing permissions for task!\n");
                                }
                                else
                                {
                                    FILE *file = fopen(results[0], "r");
                                    if (file != NULL)
                                    {
                                        char line[PATH_MAX];
                                        printf("%s\n", results[0]);
                                        while (fgets(line, sizeof(line), file))
                                        {
                                            printf("%s", line);
                                        }
                                        fclose(file);
                                    }
                                }
                            }
                        }
                        else
                        {
                            for (int i = 0; i < match_count; i++)
                            {
                                struct stat path_stat;
                                stat(results[i], &path_stat);
                                if (S_ISDIR(path_stat.st_mode))
                                {
                                    printf("\033[1;34m%s\033[0m\n", results[i]);
                                }
                                else if (S_ISREG(path_stat.st_mode))
                                {
                                    printf("\033[1;32m%s\033[0m\n", results[i]);
                                }
                            }
                        }
                    }
                    else
                    {
                        if (match_count == 0)
                        {
                            printf("No match found!\n");
                        }
                        else
                        {
                            for (int i = 0; i < match_count; i++)
                            {
                                struct stat path_stat;
                                stat(results[i], &path_stat);
                                if (S_ISDIR(path_stat.st_mode))
                                {
                                    printf("\033[1;34m%s\033[0m\n", results[i]);
                                }
                                else if (S_ISREG(path_stat.st_mode))
                                {
                                    printf("\033[1;32m%s\033[0m\n", results[i]);
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                int f2 = fork();
                if (f2 == 0)
                {
                    execvp(tokens12[0], tokens12);
                    perror("execvp");
                    exit(1);
                }
                else if (f2 > 0)
                {
                    wait(NULL);
                }
                else
                {
                    perror("fork");
                }
            }
        }
        command12 = strtok_r(NULL, ";", &save12);
    }
}
int main()
{
    gethome();
    char input[BUFFER_SIZE];

    FILE *file = fopen(logFilePath, "r");
    if (file)
    {
        while (idx < 15 && fgets(commlog[idx], sizeof(commlog[idx]), file))
        {
            commlog[idx][strcspn(commlog[idx], "\n")] = '\0';
            if (idx < 15)
            {
                idx++;
            }
        }
        fclose(file);
    }
    while (1)
    {
        int logfg = 0;
        shellprompt();
        flag2sec=0;
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }
        char ip[BUFFER_SIZE];
        strcpy(ip, input);
        input[strcspn(input, "\n")] = '\0';
        if (strstr(input, "log") != NULL)
        {
            logfg = 1;
        }
        else
            logfg = 0;
        ip[strcspn(ip, "\n")] = '\0';
        if (!logfg)
            storeloginarray(input);
        char *save1;
        char *command11 = strtok_r(ip, ";", &save1);
        while (command11 != NULL)
        {
            command11 = trim_spaces(command11);
            if (command11 == NULL || *command11 == '\0')
            {
                command11 = strtok_r(NULL, ";", &save1);
                continue;
            }

            while (strchr(command11, '&') != NULL)
            {
                char *background_command = strchr(command11, '&');
                *background_command = '\0';
                char *background_trimmed_command = trim_spaces(command11);
                if (background_trimmed_command == NULL || *background_trimmed_command == '\0')
                {
                    break;
                }

                pid_t pid = fork();
                if (pid == 0)
                {
                    printf("Background process started with PID %d\n", getpid());
                    char *args[] = {"/bin/sh", "-c", background_trimmed_command, NULL};
                    for (int i = 0; i < sizeof(args); i++)
                    {
                        if (args[i] == NULL)
                        {
                            break;
                        }
                    }
                    int x = fork();
                    if (x == 0)
                    {
                        execvp(args[0], args);
                    }
                    else if (x > 0)
                    {
                        wait(NULL);
                        printf("Background process %d finished with status %d\n", x, WEXITSTATUS(x));
                    }
                    else
                    {
                        perror("fork");
                    }
                    perror("execvp");
                    exit(1);
                }
                else if (pid > 0)
                {
                    printf("[%d] %d\n", pid, pid);
                }
                else
                {
                    perror("fork");
                }

                command11 = background_command + 1;
                command11 = trim_spaces(command11);
            }

            char *tokens11[BUFFER_SIZE];
            int tokenct1 = 0;
            char *save2;

            char *token11 = strtok_r(command11, " \t\n ", &save2);
            while (token11 != NULL)
            {
                char *c = malloc(sizeof(char) * 40000);
                c = trim_spaces(token11);
                strcpy(token11, c);
                tokens11[tokenct1++] = token11;
                token11 = strtok_r(NULL, " \t\n", &save2);
            }
            if (tokenct1 > 0 && tokens11[0] != NULL)
            {
                if (strcmp(tokens11[0], "hop") == 0)
                {
                    hop(tokens11, tokenct1);
                }
                else if (strcmp(tokens11[0], "reveal") == 0)
                {
                    char *path11 = ".";
                    int show_hidden11 = 0;
                    int show_details11 = 0;
                    for (int j11 = 1; j11 < tokenct1; j11++)
                    {
                        if (tokens11[j11][0] == '-')
                        {
                            char *flag11 = tokens11[j11] + 1;
                            while (*flag11)
                            {
                                if (*flag11 == 'a')
                                {
                                    show_hidden11 = 1;
                                }
                                else if (*flag11 == 'l')
                                {
                                    show_details11 = 1;
                                }
                                flag11++;
                            }
                        }
                        else
                        {
                            path11 = tokens11[j11];
                        }
                    }
                    reveal(path11, show_hidden11, show_details11);
                }
                else if (strcmp(tokens11[0], "proclore") == 0)
                {
                    if (tokenct1 == 1)
                    {
                        char current_pid[32];
                        snprintf(current_pid, sizeof(current_pid), "%d", getpid());
                        proclore(current_pid);
                    }
                    else if (tokenct1 == 2)
                    {
                        proclore(tokens11[1]);
                    }
                    else
                        printf("INVALID COMMAND!!\n");
                }
                else if (strcmp(tokens11[0], "seek") == 0)
                {
                    int flagexecution = 0;
                    int filesonly = 0;
                    int dirsonly = 0;
                    char *nametarget= NULL;
                    char *search_directory = ".";
                    int token_index = 1;
                    while (token_index < tokenct1 && tokens11[token_index][0] == '-')
                    {
                        for (char *fg = tokens11[token_index] + 1; *fg; ++fg)
                        {
                            if (*fg == 'd')
                            {
                                dirsonly= 1;
                            }
                            else if (*fg == 'f')
                            {
                                filesonly = 1;
                            }
                            else if (*fg == 'e')
                            {
                                flagexecution = 1;
                            }
                            else
                            {
                                printf("Invalid flag: -%c\n", *fg);
                            }
                        }
                        token_index++;
                    }

                    if (dirsonly==1 && filesonly==1)
                    {
                        printf("Invalid flags! Cannot combine -d and -f.\n");
                    }
                    else
                    {
                        if (token_index < tokenct1)
                        {
                            nametarget = tokens11[token_index];
                            token_index++;
                        }
                        else
                        {
                            printf("Usage: seek [flags] target [directory]\n");
                        }
                        if (token_index < tokenct1)
                        {
                            search_directory = expand_path(tokens11[token_index]);
                        }

                        int matchings= 0;
                        char results[1000][PATH_MAX];
                        seek_in_directory(search_directory, nametarget, dirsonly, filesonly, results, &matchings);

                        if (flagexecution)
                        {
                            if (matchings== 1)
                            {

                                struct stat statusofpath;
                                stat(results[0], &statusofpath);

                                if (S_ISDIR(statusofpath.st_mode))
                                {
                                    if (access(results[0], X_OK) != 0)
                                    {
                                        printf("Missing permissions for task!\n");
                                    }
                                    else
                                    {
                                        chdir(results[0]);
                                        printf("%s/\n", results[0]);
                                    }
                                }
                                else if (S_ISREG(statusofpath.st_mode))
                                {
                                    if (access(results[0], R_OK) != 0)
                                    {
                                        printf("Missing permissions for task!\n");
                                    }
                                    else
                                    {
                                        FILE *file = fopen(results[0], "r");
                                        if (file != NULL)
                                        {
                                            char line[PATH_MAX];
                                            printf("%s\n", results[0]);
                                            while (fgets(line, sizeof(line), file))
                                            {
                                                printf("%s", line);
                                            }
                                            fclose(file);
                                        }
                                    }
                                }
                            }
                            else
                            {
                                int pointer=0;
                                while(pointer<matchings) 
                                {
                                    struct stat statusofpath;
                                    stat(results[pointer], &statusofpath);
                                    if (S_ISDIR(statusofpath.st_mode))
                                    {
                                        printf("\033[1;34m%s\033[0m\n", results[pointer]);
                                    }
                                    else if (S_ISREG(statusofpath.st_mode))
                                    {
                                        printf("\033[1;32m%s\033[0m\n", results[pointer]);
                                    }
                                    pointer++;
                                }
                            }
                        }
                        else
                        {
                            if (matchings== 0)
                            {
                                printf("No match found!\n");
                            }
                            else
                            {
                                int pointer2=0;
                                while(pointer2<matchings)
                                {
                                    struct stat pathstatus2;
                                    stat(results[pointer2], &pathstatus2);
                                    if (S_ISDIR(pathstatus2.st_mode))
                                    {
                                        printf("\033[1;34m%s\033[0m\n", results[pointer2]);
                                    }
                                    else if (S_ISREG(pathstatus2.st_mode))
                                    {
                                        printf("\033[1;32m%s\033[0m\n", results[pointer2]);
                                    }
                                    pointer2++;
                                }
                            }
                        }
                    }
                }

                else if (strcmp(tokens11[0], "log") == 0)
                {
                    if (tokenct1 == 2)
                    {
                        if (strcmp(tokens11[1], "purge") == 0)
                        {
                            idx = 0;
                            savetofile();
                        }
                    }
                    else if (tokenct1 == 1)
                    {
                        FILE *file;
                        char line[MAX_STRING_LENGTH];

                        file = fopen(logFilePath, "r");
                        if (file == NULL)
                        {
                            perror("Error opening file");
                            return 1;
                        }

                        while (fgets(line, sizeof(line), file) != NULL)
                        {
                            printf("%s", line);
                        }

                        fclose(file);
                    }
                    else if (tokenct1 == 3)
                    {
                        if (strcmp(tokens11[1], "execute") == 0)
                        {
                            int execindex = 0;
                            execindex = atoi(tokens11[2]);
                            execindex = idx - execindex;
                            executelog(commlog[execindex]);
                        }
                    }
                }
                else
                {
                    int f = fork();
                    if (f == 0)
                    {
                        execvp(tokens11[0], tokens11);
                        perror("INVALID COMMAND");
                        exit(1);
                    }
                    else if (f > 0)
                    {
                        int status;
                        struct timeval start, end;
                        gettimeofday(&start, NULL);
                        wait(NULL);
                        gettimeofday(&end, NULL);
                        double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
                        if (time_taken > 2.00)
                        {
                            timeprocess = (int)time_taken;
                            flag2sec = 1;
                            strcpy(command2sec, command11);
                        }
                        // printf("%d",timeprocess);
                    }
                    else
                    {
                        perror("fork");
                    }
                }
            }

            command11 = strtok_r(NULL, ";", &save1);
        }
    }
    return 0;
}
