#ifndef ACTIVITIES_H
#define ACTIVITIES_H

#include "baapheader.h"

void push_process(pid_t pid, char* commname, int state);
int comparator(const void* a,const void* b);
void delete_process(int pid);
void sendsignalpid(int pid,int signal);
void spec13();
void status_of_process();
#endif 
