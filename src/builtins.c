#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "builtins.h"

BuiltInCommand get_builtin(char *cmd) {
    if (strcmp(cmd, "cd") == 0)
        return BUILTIN_CD;

    if (strcmp(cmd, "exit") == 0)
        return BUILTIN_EXIT;
    
    if (strcmp(cmd, "help") == 0)
        return BUILTIN_HELP;


    return BUILTIN_NONE;
}

void change_directory(char* new_directory, char* cwd, int path_buffer_size) {
    if (new_directory == NULL) {
        chdir(getenv("HOME"));
    } else if (chdir(new_directory) != 0) {
        perror("cd");
        return;
    }
    
    getcwd(cwd, path_buffer_size);        
}

void print_help(void) {
    printf("Built-in commands:\n");
    printf("cd [directory]\n");
    printf("help\n");
    printf("exit\n");
}
