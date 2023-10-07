#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIMPLE_DB_VERSION "1.0"
#define BUFFER_SIZE 256

typedef struct {
    char command[BUFFER_SIZE];
} Command;

void print_prompt() {
    printf("SimpleDB > ");
}

void read_command(Command* cmd) {
    fgets(cmd->command, BUFFER_SIZE, stdin);
    cmd->command[strcspn(cmd->command, "\n")] = '\0';  // Remove trailing newline

    if (strcmp(cmd->command, ".exit") == 0) {
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char* argv[]) {
    printf("SimpleDB version %s starting..\n",SIMPLE_DB_VERSION);
    while (true) {
        print_prompt();
        Command cmd;
        read_command(&cmd);

        if (strcmp(cmd.command, "get") == 0) {
            printf("Get: %s\n",cmd.command);
        } else if (strcmp(cmd.command, "set") == 0) {
            printf("set: \n");
        } else if (strcmp(cmd.command, "delete") == 0) {
            printf("delete: %s\n", cmd.command);
        } else {
            printf("Unknown command: %s\n", cmd.command);
        }
    }

    return 0;
}
