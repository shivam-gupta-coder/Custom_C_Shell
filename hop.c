#include "baapheader.h"
void changedirectory(const char *path)
{
    char newdir[PATH_MAX];
    if (chdir(path) == 0)
    {
        getcwd(newdir, sizeof(newdir));
        strcpy(prevdir, currdir);
        strcpy(currdir, newdir);
    }
    else
    {
        perror("Could not change directory. SORRY!!");
    }
}

void hop(char *tokens[], int token_count)
{
    char homedir[PATH_MAX];
    strcpy(homedir, home);

    for (int i = 1; i < token_count; i++)
    {
        char *token = tokens[i];

        if (strcmp(token, "~") == 0)
        {
            changedirectory(homedir);
        }
        else if (strcmp(token, "-") == 0)
        {
            if (strlen(prevdir) > 0)
            {
                changedirectory(prevdir);
            }
            else
            {
                printf("No previous directory to hop to.\n");
                continue;
            }
        }
        else if (strcmp(token, "..") == 0)
        {
            changedirectory("..");
        }
        else if (strcmp(token, ".") == 0)
        {
            // Do nothing for "."
        }
        else
        {
            if (token[0] == '/')
            {
                changedirectory(token);
            }
            else if (strncmp(token, "~/", 2) == 0)
            {
                char fullPath[PATH_MAX];
                snprintf(fullPath, sizeof(fullPath), "%s%s", homedir, token + 1);
                changedirectory(fullPath);
            }
            else
            {
                changedirectory(token);
            }
        }

        getcwd(currdir, sizeof(currdir));
        printf("%s\n", currdir);
    }
}