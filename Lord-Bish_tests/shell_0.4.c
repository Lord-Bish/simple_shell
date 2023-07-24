#include "main.h"

#define MAX_INPUT_LENGTH 100

extern char **environ;

void display_prompt() {
    printf("($) ");
    fflush(stdout);
}

void execute_command(char* command) {
    char *args[3];
    int status;
    pid_t pid;

    if (strcmp(command, "exit") == 0) {
        exit(EXIT_SUCCESS);
    } else if (strcmp(command, "env") == 0) {
        char **env = environ;
        while (*env) {
            printf("%s\n", *env);
            env++;
        }
        return;
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /* Child process */
        args[0] = command;
        args[1] = NULL;

        /* Check if the command exists in the PATH */
        if (access(command, X_OK) != 0) {
            printf("./hsh: 1: %s: command not found\n", args[0]);
            exit(EXIT_FAILURE);
        }

        if (execve(command, args, environ) == -1) {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    } else {
        /* Parent process */
        waitpid(pid, &status, 0);
    }
}

int main() {
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t chars_read;

    while (1) {
        display_prompt();
       
        chars_read = getline(&line, &bufsize, stdin);

        if (chars_read == -1) {
            /* End of file (Ctrl+D) or error occurred */
            if (feof(stdin)) {
                printf("\n");
                break;
            } else {
                perror("getline");
                exit(EXIT_FAILURE);
            }
        }

        /* Remove the trailing newline character */
        line[strcspn(line, "\n")] = '\0';

        execute_command(line);
    }

    free(line);
    exit(EXIT_SUCCESS);
}
