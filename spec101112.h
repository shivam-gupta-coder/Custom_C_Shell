#ifndef SPEC101112_H
#define SPEC101112_H
#include "baapheader.h"

void handle_sigint2(int sig);
void handle_sigtstp2(int sig);
void handle_sigchld2(int sig);
void setup_signal_handlers2();
void spec10112(char* command);
void spec101122(char* command);
void ampercent(char* command);

#endif 