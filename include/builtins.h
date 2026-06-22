typedef enum {
    BUILTIN_CD,
    BUILTIN_EXIT,
    BUILTIN_HELP,
    BUILTIN_NONE,
} BuiltInCommand;

BuiltInCommand get_builtin(char* cwd);
void change_directory(char* new_directory, char* cwd, int path_buffer_length);
void print_help(void);