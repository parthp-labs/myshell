#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

void execute_external(char *argv[], int background, int redirect_output, char output_file[], int redirect_input, char input_file[])
{
    pid_t pid = fork();
    
    if (background == 1 && pid != 0) {
        printf("[Background PID: %d]\n", pid);
    }

    if (pid == 0) {
        // Redirecting the child's output to the file
        // | is bitwise OR. All three flags are enabled simultaneously
        if (redirect_output != 0) {
            int fd;
            
            if (redirect_output == 1) {
                fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            } else {
                fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            }
            
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }

            if (dup2(fd, STDOUT_FILENO) == -1) {
                perror("dup2");
                close(fd);
                exit(EXIT_FAILURE);
            }
            close(fd);
        } else if (redirect_input == 1) {
            int fd = open(input_file, O_RDONLY);

            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }

            if (dup2(fd, STDIN_FILENO) == -1) {
                perror("dup2");
                close(fd);
                exit(EXIT_FAILURE);
            }
            close(fd);
        }

        int process_status = execvp(argv[0], argv);

        if (process_status == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("fork");
    } else if (background != 1) {
        waitpid(pid, NULL, 0);
    }
}