#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))

#include <stdio.h>
#include <string.h>

typedef struct {
  const char *name;
  const char *description;
  int min_args;
  int max_args;
  void (*execute)(char **args);
} Command;

void cmd_cd(char **args) { printf("Changing directory to: %s\n", args[1]); }

void cmd_ls(char **args) { printf("Listing directory contents.\n"); }

void cmd_exit(char **args) { printf("Exiting shell.\n"); }

Command commands[] = {{"cd", "Change directory", 1, 1, cmd_cd},
                      {"ls", "List directory contents", 0, 0, cmd_ls},
                      {"exit", "Exit the shell", 0, 0, cmd_exit}};

void execute_command(const char *input) {
  char *args[10];
  char input_copy[100];
  strcpy(input_copy, input);

  int argc = 0;
  char *token = strtok(input_copy, " ");
  while (token != NULL && argc < 10) {
    args[argc++] = token;
    token = strtok(NULL, " ");
  }
  args[argc] = NULL;

  for (int i = 0; i < COMMAND_COUNT; i++) {
    if (strcmp(args[0], commands[i].name) == 0) {
      if (argc - 1 >= commands[i].min_args &&
          argc - 1 <= commands[i].max_args) {
        commands[i].execute(args);
        return;
      } else {
        printf("Usage: %s\n", commands[i].description);
        return;
      }
    }
  }

  printf("%s: command not found\n", args[0]);
}

int main() {
  // Flush after every printf
  setbuf(stdout, NULL);

  char input[100];

  while (1) {
    printf("$ ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) {
      execute_command(input);
    }
  }
  return 0;
}
