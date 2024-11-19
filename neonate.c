#include "baapheader.h"
static struct termios original_termios;
void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

void enable_raw_mode() {
    struct termios raw_termios;
    tcgetattr(STDIN_FILENO, &original_termios);
    atexit(disable_raw_mode);

    raw_termios = original_termios;
    raw_termios.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw_termios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw_termios.c_cflag |= CS8;
    raw_termios.c_oflag &= ~OPOST;
    raw_termios.c_cc[VMIN] = 1;
    raw_termios.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &raw_termios);
}

void neonate(int interval) {
    if (interval <= 0) {
        fprintf(stderr, "Error: Interval must be greater than 0.\n");
        return;
    }

    enable_raw_mode();

    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        disable_raw_mode();
        return;
    }

    if (child_pid == 0) {
        while (1) {
            FILE *pid_file = fopen("/proc/sys/kernel/ns_last_pid", "r");
            if (pid_file == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }

            char pid_buffer[16];
            if (fgets(pid_buffer, sizeof(pid_buffer), pid_file) != NULL) {
                pid_buffer[strcspn(pid_buffer, "\n")] = '\0';
                printf("\r%s\n", pid_buffer);
            }
            fflush(stdout);
            fclose(pid_file);
            
            sleep(interval);
        }
    } else {
        char input_char;
        while (read(STDIN_FILENO, &input_char, 1) == 1 && input_char != 'x');
        kill(child_pid, SIGKILL);
    }

    disable_raw_mode();
}