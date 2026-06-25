#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "builtins.h"
#include "executor.h"
#include "parser.h"

#define string char*
#define PATH_BUFFER 1024

// Parsing steps
// Separate using |
// Then create argv for each right and left
// Then run exec for each section based on pipe()

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

        struct pipe_commands pipe_cmd = parse_pipe(buffer);
        
        if (pipe_cmd.right_args != NULL) {
            execute_pipe(pipe_cmd);
        } else {
            char** argv = malloc(100);
            parse_args(buffer, argv);

            struct command result = parse_redirection(argv);
            
            // for (int i=0;i<arg_index;i++) {
            //     printf("Token %i:%s\n", i, result.args[i]);
            // }
            // printf("Executing command\n");

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
                    execute_external(result, background);
                    break;
            }        
        }  
    }

    return 0;
}