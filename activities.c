#include "baapheader.h"
typedef struct bgprocesses {
    int pid;
    char commname[4096];
    int state;
} bgprocesses;

bgprocesses array[4096];
void push_process(pid_t pid, char* commname, int state) {
    array[pcount].pid = pid;
    strcpy(array[pcount].commname, commname);
    array[pcount].state = state;
    pcount++;
}

int comparator(const void *a, const void *b) {
    return ((bgprocesses *)a)->commname- ((bgprocesses *)b)->commname;
}
void delete_process(int target_pid) {
    int found = 0;
    int target_index = 0;
    int current_index = 0;

    while (current_index < pcount) { 
        if (array[current_index].pid == target_pid) {
            target_index = current_index;
            found = 1;
            break;
        }
        current_index++;
    }

    if (found) { 
        int shift_index = target_index;
        while (shift_index < pcount - 1) {
            array[shift_index] = array[shift_index + 1];
            shift_index++;
        }
        pcount--;
    }
}

void sendsignalpid(int pid, int signal) {
    if (kill(pid, 0) == -1) {
        perror("ERROR");
        return;
    }
    if (kill(pid, signal) == 0) {
        printf("Signal %d sent successfully to process with pid %d\n", signal, pid);
    } else {
        perror("ERROR");
    }
}

void spec13() {
    qsort(array, pcount, sizeof(bgprocesses), comparator);
    for (int i = 0; i < pcount; i++) {
        printf("%d %s %s\n", array[i].pid, array[i].commname, array[i].state == 1 ? "Running" : "Stopped");
    }
}
void status_of_process() {
    char procid[32];
    int current_index = 0;

    while (current_index < pcount) {
        procid[strcspn(procid, "\n")] = 0;
        char path[256];
        snprintf(path, sizeof(path), "/proc/%d/stat", array[current_index].pid);
        
        FILE *stat_file = fopen(path, "r");
        if (stat_file == NULL) {
            delete_process(array[current_index].pid);
            current_index++;
            continue;
        }

        char command13[256];
        char status;
        int ppid;
        fscanf(stat_file, "%d %s %c", &ppid, command13, &status);
        fclose(stat_file);
        
        const char *stat15;

        if (status == 'R') {
            stat15 = "Running";
            array[current_index].state = 1;
        } else if (status == 'S') {
            stat15 = "Sleeping";
            array[current_index].state = 1;
        } else if (status == 'D') {
            stat15 = "Waiting";
            array[current_index].state = 0;
        } else if (status == 'Z') {
            stat15 = "Zombie";
            delete_process(array[current_index].pid);
        } else if (status == 'T') {
            stat15 = "Stopped";
            array[current_index].state = 0;
            printf("Stopped Process\n");
        } else {
            stat15 = "Unknown";
            delete_process(array[current_index].pid);
        }
        
        current_index++;
    }
}
