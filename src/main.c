#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

#define string char*

int main() {
    char buffer[50];
    int process_status = 0;

    while (1) {
        printf("myshell> ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\nGoodbye!\n");
            break;
        }

        // Overwriting the newline character with null terminator
        for (int i=0;i<sizeof(buffer);i++) {
            if (buffer[i] == '\n') {
                buffer[i] = '\0';
                break;
            }
        }

        if (strcmp(buffer, "exit") == 0) {
            printf("Goodbye !\n");
            break;
        }

        if (buffer[0] == '\0') {
            continue;
        }

        // Tokenizing the arguments
        string argv[10];
        int arg_index = 0;

        char *t = strtok(buffer, " ");

        while (t != NULL && arg_index < 9) {
            argv[arg_index] = t;
            arg_index++;
            t = strtok(NULL, " ");
        }
        argv[arg_index] = NULL;

        for (int i=0;i<arg_index;i++) {
            printf("Token %d: %s\n", i, argv[i]);
        }

        // Running commands
        pid_t pid = fork();

        if (pid == 0) {
            process_status = execvp(argv[0], argv);

            if (process_status == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else if (pid < 0) {
            perror("fork");
        } else {
            waitpid(pid, NULL, 0);
        }
        
    }

    return 0;
}