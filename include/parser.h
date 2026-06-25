#ifndef PARSER_H
#define PARSER_H

struct pipe_commands {
    char** left_args;
    char** right_args;
};


struct command {
    char** args;
    char output_file[40];
    char input_file[40];
    int redirect_input;
    int redirect_output;
};

void parse_args(char text[], char** out);

struct command parse_redirection(char** argv);

struct pipe_commands parse_pipe(char buffer[]);

#endif