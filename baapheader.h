#ifndef BAAP_HEADER_H
#define BAAP_HEADER_H
#define _DEFAULT_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <time.h>
#include <dirent.h>
#include <grp.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <libgen.h>
#include <wordexp.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#define LOG_NUM 15
#define MAX_STRING_LENGTH 1000
#define MAX_LINE_LENGTH 1024
#define BUFFER_SIZE 1024
#define GREEN "\033[92m"
#define BLUE "\033[94m"
#define RED "\033[31m"
#define RESET "\033[0m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_RESET "\033[0m"
#define MAX_ALIAS_COMMAND_LENGTH 100
#define MAX_ALIAS_NAME_LENGTH 100
#define MAX_ALIASES 1000
// #ifndef PATH_MAX
#define PATH_MAX 4096
// #endif
extern char* logFilePath;             // Declare the variable as extern
extern char commlog[LOG_NUM][MAX_STRING_LENGTH]; // Declare the variable as extern
extern int commands;                           // Declare the variable as extern
extern int idx;                                // Declare the variable as extern
extern char currdir[PATH_MAX];                 // Declare the variable as extern
extern char prevdir[PATH_MAX];                 // Declare the variable as extern
extern char home[PATH_MAX];                    // Declare the variable as extern
extern int flag2sec;
extern char command2sec[PATH_MAX];
extern int timeprocess;
extern char* pids;
extern int pcount;
extern int neo_flag;
extern pid_t fg_pid;
extern int alias_count;
extern int is_alias;                                                                                                                                                                                                                                                         
#include "trimspaces.h"
#include "display.h"
#include "hop.h"
#include "reveal.h"
#include "proclore.h"
#include "log.h"
#include "seek.h"
#include "activities.h"
#include "spec101112.h"
#include "neonate.h"
#include "fgbg.h"
#include "signal.h"
#include "alias.h"
#include "iman2.h"

#endif