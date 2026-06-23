#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void execute_external(char *argv[])
{
    pid_t pid = fork();

    if (pid == 0) {
        int process_status = execvp(argv[0], argv);

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