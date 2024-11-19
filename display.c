#include "baapheader.h"

void gethome()
{
    if (getcwd(currdir, sizeof(currdir)) != NULL)
    {
        strcpy(home, currdir);
        strcpy(prevdir, currdir);
        
    }
}
void shellprompt()
{
    char username[BUFFER_SIZE];
    char systemname[BUFFER_SIZE];
    char modifieddir[PATH_MAX];

    getlogin_r(username, BUFFER_SIZE);
    gethostname(systemname, BUFFER_SIZE);

    char current[PATH_MAX];
    getcwd(current, sizeof(current));
    if (strncmp(current, home, strlen(home)) == 0)
    {
        snprintf(modifieddir, sizeof(modifieddir), "~%s", current + strlen(home));
    }
    else
    {
        strcpy(modifieddir, current);
    }
    if (flag2sec==1)
        printf("<%s@%s:%s %s:%d>", username, systemname, modifieddir, command2sec, timeprocess);
    else
    {
      printf("<%s@%s:%s> ", username, systemname, modifieddir);
    }
}
