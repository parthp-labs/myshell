#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

void execute_external(struct command result, int background);

void execute_pipe(struct pipe_commands pipe_cmd);

#endif