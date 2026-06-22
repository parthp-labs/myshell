#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "builtins.h"
#include "execute_external.h"

#define string char*
#define PATH_BUFFER 1024

int main() {
    char buffer[50];
    char cwd[PATH_BUFFER];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
    }

    while (1) {
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
        int arg_index = 0;

        char *t = strtok(buffer, " ");

        while (t != NULL && arg_index < 9) {
            argv[arg_index] = t;
            arg_index++;
            t = strtok(NULL, " ");
        }
        argv[arg_index] = NULL;

        // for (int i=0;i<arg_index;i++) {
        //     printf("Token %d: %s\n", i, argv[i]);
        // }

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
                execute_external(argv);
                break;
        }        
    }

    return 0;
}