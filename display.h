#ifndef DISPLAY_H
#define DISPLAY_H

#include "baapheader.h"


extern char currdir[PATH_MAX];
extern char homedir[PATH_MAX];
extern char prevdir[PATH_MAX];
void gethome();
void shellprompt();


#endif