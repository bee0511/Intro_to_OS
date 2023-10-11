#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        // Print the shell prompt
        printf("osh> ");
        fflush(stdout);

        // Read user input
        if (fgets(command, sizeof(command), stdin) == NULL) {
            perror("Failed to read command");
            exit(EXIT_FAILURE);
        }

        // Remove newline character from the input
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }

        // Check for the exit command
        if (strcmp(command, "exit") == 0) {
            printf("Process end\n");
            break;
        }

        // Fork a child process
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            // Tokenize the command
            char *args[MAX_COMMAND_LENGTH];
            char *token = strtok(command, " ");
            int i = 0;
            while (token != NULL) {
                args[i] = token;
                token = strtok(NULL, " ");
                i++;
            }
            args[i] = NULL;

            // Execute the command using execvp
            if (execvp(args[0], args) == -1) {
                perror("Command execution failed");
                exit(EXIT_FAILURE);
            }
        } else { // Parent process
            // Wait for the child to complete
            int status;
            waitpid(pid, &status, 0);

            // if (WIFEXITED(status)) {
               //  printf("Child process exited with status %d\n", WEXITSTATUS(status));
            // } else {
               //  printf("Child process did not exit normally\n");
            //}
        }
    }

    return 0;
}
