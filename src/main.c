#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "builtins.h"
#include "executor.h"

#define string char*
#define PATH_BUFFER 1024

int main() {
    char buffer[50];
    char cwd[PATH_BUFFER];
    int background = 0;
    int redirect_output = 0;
    int redirect_input = 0;

    pid_t finished_pid;

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
    }

    while (1) {
        background = 0;
        redirect_output = 0;
        redirect_input = 0;

        // Checking for any finished child processes
        // -1 means, wait for any child process
        // The -1 is a special argument telling the kernel:
        //"I don't care which child. Give me any completed child."
        while ((finished_pid = waitpid(-1, NULL, WNOHANG)) > 0) {
            printf("[Background PID %d finished]\n", finished_pid);
        }
        
        printf("[%s]> ", cwd);

        if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Goodbye!\n");
            break;
        }

        // Overwriting the newline character with null terminator
        for (int i=0;i<(int)sizeof(buffer);i++) {
            if (buffer[i] == '\n') {
                buffer[i] = '\0';
                break;
            }
        }

        if (buffer[0] == '\0') {
            continue;
        }

        // Tokenizing the arguments
        string argv[10];
        char output_file[256];
        char input_file[256];
        int arg_index = 0;

        char *t = strtok(buffer, " ");

        while (t != NULL && arg_index < 9) {
            if (strcmp(t, "&") == 0) {
                background = 1;
            } else {
                argv[arg_index] = t;
                arg_index++;
            }
        
            t = strtok(NULL, " ");
        }

        argv[arg_index] = NULL;

      
        // Checking for redirects
        for (int i=0;i<arg_index;i++) {
            if (strcmp(argv[i], ">") == 0) {
                redirect_output = 1;
            
                if (argv[i+1] == NULL) {
                    printf("Missing output file\n");
                    break;
                }
                
                strcpy(output_file, argv[i+1]);
                argv[i] = NULL;
                argv[i+1] = NULL;
                i++;
                arg_index-=2;
                break;
            } else if (strcmp(argv[i], ">>") == 0) {
                redirect_output = 2;
            
                if (argv[i+1] == NULL) {
                    printf("Missing output file\n");
                    break;
                }
                
                strcpy(output_file, argv[i+1]);
                argv[i] = NULL;
                argv[i+1] = NULL;
                i++;
                arg_index-=2;
                break;
            } else if (strcmp(argv[i], "<") == 0) {
                redirect_input = 1;
      
                if (argv[i+1] == NULL) {
                    printf("Missing input file\n");
                    break;
                }
            
                strcpy(input_file, argv[i+1]);
                argv[i] = NULL;
                argv[i+1] = NULL;
                i++;
                arg_index-=2;
                break;

            }
        }

           for (int i=0;i<5;i++) {
            printf("Token %d: %s\n", i, argv[i]);
        }


        // Handling builtin commands
        BuiltInCommand command = get_builtin(argv[0]);
        
        switch(command) {
            case BUILTIN_CD:
                change_directory(argv[1], &cwd[0], PATH_BUFFER);
                break;
            case BUILTIN_EXIT:
                printf("Goodbye!\n");
                return 0;
            case BUILTIN_HELP:
                print_help();
                break;
            default:
                // Running external commands
                execute_external(argv, background, redirect_output, output_file, redirect_input, input_file);
                break;
        }        
    }

    return 0;
}