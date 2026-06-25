#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "parser.h"

void execute_external(struct command result, int background)
{
    pid_t pid = fork();

    if (background == 1 && pid != 0) {
        printf("[Background PID: %d]\n", pid);
    }

    if (pid == 0) {
        // Redirecting the child's output to the file
        // | is bitwise OR. All three flags are enabled simultaneously
        if (result.redirect_output != 0) {
            int fd;
            
            if (result.redirect_output == 1) {
                fd = open(result.output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            } else {
                fd = open(result.output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
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
        } else if (result.redirect_input == 1) {
            int fd = open(result.input_file, O_RDONLY);

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

        int process_status = execvp(result.args[0], result.args);

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

void execute_pipe(struct pipe_commands pipe_cmd) {
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    };

    pid_t pid1 = fork();

    if (pid1 == 0) {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        if (execvp(pipe_cmd.left_args[0], pipe_cmd.left_args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } 

    pid_t pid2 = fork();

    if (pid2 == 0) {
        close(pipe_fd[1]);  
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
        if (execvp(pipe_cmd.right_args[0], pipe_cmd.right_args) == -1)  {
            perror("execvp");
            exit(EXIT_FAILURE);
        } 
    } 

    close(pipe_fd[0]);
    close(pipe_fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}