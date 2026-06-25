#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"


void parse_args(char text[], char** out) {
    char * saveptr;
    char* token = strtok_r(text, " ", &saveptr);
    int i = 0;

    while(token != NULL) {
        out[i] = malloc((strlen(token) + 1) * sizeof(char));

        strcpy(out[i], token);
        i++;
        token = strtok_r(NULL, " ", &saveptr);
    }
    out[i] = NULL;
}

struct command parse_redirection(char** argv) {
    struct command result = {0};
    int result_arg_index = 0;

    result.args = malloc(100 * sizeof(char*));

    // Checking for redirects
    int i = 0;
    char* val = argv[i];
    while (val != NULL) {
        if (strcmp(val, ">") == 0) {
            result.redirect_output = 1;
            
            if (argv[i+1] == NULL) {
                printf("Missing output file\n");
                break;
            }
            
            strcpy(result.output_file, argv[i+1]); 
            break;
        } else if (strcmp(val, ">>") == 0) {
            result.redirect_output = 2;

            if (argv[i+1] == NULL) {
                printf("Missing ouput file\n");
                break;
            }

            strcpy(result.output_file, argv[i+1]);
            break;
        }
        else if (strcmp(val, "<") == 0) {
            result.redirect_input = 1;

            if (argv[i+1] == NULL) {
                printf("Missing input file\n");
                break;
            }

            strcpy(result.input_file, argv[i+1]);
            break;
        }

        result.args[i] = malloc(20);

        strcpy(result.args[i], val);
        i++;
        val = argv[i];
    }


    // int index = 0;
    // while (result.args[index] != NULL) {
    //     printf("Token %d: %s\n", index, result.args[index]);
    //     index++;
    // }

    // printf("%s\n", result.output_file);
    return result;
}

struct pipe_commands parse_pipe(char buffer[]) {
    struct pipe_commands pipe_cmd = {NULL, NULL};
    char *saveptr;

    char left_str[50] = {0};
    char right_str[50] = {0};

    char* t = strtok_r(buffer, "|", &saveptr);

    if (t != NULL) {
        strcpy(left_str, t);

        t = strtok_r(NULL, "|", &saveptr);
        if (t != NULL) {
            if (*t == ' ') t++;
            strcpy(right_str, t);
        } else {
            return pipe_cmd;
        }
    }

    // printf("After | parsing: \nLeft:%s\n", left_str);
    // printf("Right:%s\n", right_str);

    pipe_cmd.left_args = malloc(20 * sizeof(char*));
    pipe_cmd.right_args = malloc(20 * sizeof(char*));

    parse_args(left_str, pipe_cmd.left_args);
    parse_args(right_str, pipe_cmd.right_args);
    
    return pipe_cmd;
}
