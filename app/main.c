#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  const char *name;
  const char *description;
  int min_args;
  int max_args;
  void (*execute)(char **args);
} Command;

void cmd_cd(char **args);
void cmd_ls(char **args);
void cmd_exit(char **args);
void cmd_echo(char **args);
void cmd_type(char **args);

Command commands[] = {
    {"cd", "Change directory", 1, 1, cmd_cd},
    {"ls", "List directory contents", 0, 0, cmd_ls},
    {"exit", "Exit the shell", 0, 1, cmd_exit},
    {"echo", "Display a line of text", 1, 20, cmd_echo}, // TODO: max_arg prob
    {"type", "Display details about command", 1, 1, cmd_type}, // BUG: same
}; // add type, rem min/max

Command *get_command(const char *command_name) {
  int num_commands = sizeof(commands) / sizeof(commands[0]);
  for (int i = 0; i < num_commands; i++) {
    if (strcmp(commands[i].name, command_name) == 0) {
      return &commands[i];
    }
  }
  return NULL;
}

void cmd_cd(char **args) { printf("Changing directory to: %s\n", args[1]); }
void cmd_ls(char **args) { printf("Listing directory contents.\n"); }

void cmd_exit(char **args) {
  if (args[1] != NULL) {
    exit(atoi(args[1]));
  }
  exit(0);
}

void cmd_echo(char **args) {
  char echo_stomp[1024] = ""; // max_arg prob
  int i = 1;
  while (args[i] != NULL) {
    if (i > 1) {
      strcat(echo_stomp, " ");
    }
    strcat(echo_stomp, args[i]);
    i++;
  }
  printf("%s\n", echo_stomp);
}

void cmd_type(char **args) {
  Command *cmd = get_command(args[1]);
  if (cmd)
    printf("%s is a shell builtin\n", args[1]);
  else
    printf("%s: not found\n", args[1]);
}
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
