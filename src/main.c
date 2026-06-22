#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define string char*
#define PATH_BUFFER 1024

typedef enum {
    BUILTIN_CD,
    BUILTIN_EXIT,
    BUILTIN_HELP,
    BUILTIN_NONE,
} BuiltInCommand;

BuiltInCommand get_builtin(char *cmd) {
    if (strcmp(cmd, "cd") == 0)
        return BUILTIN_CD;

    if (strcmp(cmd, "exit") == 0)
        return BUILTIN_EXIT;
    
    if (strcmp(cmd, "help") == 0)
        return BUILTIN_HELP;


    return BUILTIN_NONE;
}

void change_directory(char* new_directory, char* cwd) {
    if (new_directory == NULL) {
        chdir(getenv("HOME"));
    } else if (chdir(new_directory) != 0) {
        perror("cd");
        return;
    }
    
    getcwd(cwd, PATH_BUFFER);        
}

void print_help(void)
    {
        printf("Built-in commands:\n");
        printf("cd [directory]\n");
        printf("help\n");
        printf("exit\n");
    }


int main() {
    char buffer[50];
    int process_status = 0;
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
        for (int i=0;i<sizeof(buffer);i++) {
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
                change_directory(argv[1], &cwd[0]);
                break;
            case BUILTIN_EXIT:
                printf("Goodbye !");
                return 0;
            case BUILTIN_HELP:
                print_help();
                break;
            default:
                // Running external commands
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
                break;
        }        
    }

    return 0;
}