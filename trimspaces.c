#include "baapheader.h"

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