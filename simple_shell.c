#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    char *cmd;
    size_t bufsize = 32;
    ssize_t characters;
    char *args[] = {NULL};

    while (1) {
        printf("#cisfun$ ");
        cmd = (char *)malloc(bufsize * sizeof(char));
        if (cmd == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }

        characters = getline(&cmd, &bufsize, stdin);
        if (characters == -1) {
            if (feof(stdin)) {
                printf("\n");
                break; // End of file (Ctrl+D)
            } else {
                perror("getline failed");
                exit(EXIT_FAILURE);
            }
        }

        cmd[characters - 1] = '\0'; // Remove newline

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // Child process
            if (execve(cmd, args, NULL) == -1) {
                perror("execve failed");
                exit(EXIT_FAILURE);
            }
        } else { // Parent process
            wait(NULL);
        }

        free(cmd);
    }

    return 0;
}
