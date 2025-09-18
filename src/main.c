#define _GNU_SOURCE // enables gethostname
#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

const int DEBUG = 0;

int main()
{
  while (1) {
    // get $USER, $MACHINE, $PWD env variables
    // Issue #20 fix: Prompt shows (null) on non-Fedora systems
    char *user = getenv("USER");
    char *machine = getenv("MACHINE");
    char hostname[256];
    if (!machine) {
      // fallback to hostname if MACHINE not set
      if (gethostname(hostname, sizeof(hostname)) == 0) {
        machine = hostname;
      } else {
        machine = "localhost";  // sensible default
      }
    }
    char *pwd = getenv("PWD");

    printf("%s@%s:%s> ", user, machine, pwd);

    /* input contains the whole command
     * tokens contains substrings from input split by spaces
     */

    char *input = get_input();

    /* Issue #18 fix: Add EOF handling for Part 6 compatibility
     *  Also see lexer.c fix, the starter file actually causes 
     *  this, not our code. Should exit cleanly upon EOF now.
     */
    if (input == NULL) {
      printf("\n");  // print newline for clean exit
      break;  // exit the shell on EOF
    }

    if (DEBUG) printf("[DEBUG] Whole input: %s\n", input);

    tokenlist *tokens = get_tokens(input);
    int num_tokens = tokens->size;

    // if there's no input, just continue
    if (num_tokens == 0) {
      free(input);
      free_tokens(tokens);
      continue;
    }

    // replace tokens like $USER or ~
    for (int i = 0; i < num_tokens; i++) {
      if (tokens->items[i][0] == '~' && (tokens->items[i][1] == '/' || tokens->items[i][1] == '\0')) {
        // Replace ~ with $HOME
        char* home = getenv("HOME");
        if (home) {
          char* new_token = malloc(strlen(home) + strlen(tokens->items[i]) + 1);
          strcpy(new_token, home);
          strcat(new_token, tokens->items[i] + 1); // skip '~'
          free(tokens->items[i]);
          tokens->items[i] = new_token;
        }
        continue;
      } else if (tokens->items[i][0] == '$') {
        char* var_name = tokens->items[i] + 1;  // skip '$'
        char* var_value = getenv(var_name);
        if (var_value) {
          free(tokens->items[i]);
          char* new_token = malloc(strlen(var_value) + 1);
          strcpy(new_token, var_value);
          tokens->items[i] = new_token;
        }
      }
    }

    const char* first_token = tokens->items[0];
    char* exec_path = NULL;

    // Issue #19 fix: Commands with slashes incorrectly use PATH search
    if (strchr(first_token, '/') != NULL) {
      // command contains a slash - use it directly (absolute or relative path)
      exec_path = malloc(strlen(first_token) + 1);
      strcpy(exec_path, first_token);
      if (DEBUG) printf("[DEBUG] Direct execution (has slash): %s\n", exec_path);
    } else {
      // no slash - search $PATH for the command
      const char* path = getenv("PATH");
      char* path_copy = malloc(strlen(path) + 1);
      strcpy(path_copy, path);

      char* token = strtok(path_copy, ":");
      while (token != NULL) {
        // check if executable exists in this path for tokens[0]
        exec_path = malloc(strlen(token) + strlen(first_token) + 2);
        sprintf(exec_path, "%s/%s", token, first_token);

        if (DEBUG) printf("[DEBUG] Checking path: %s\n", exec_path);

        if (access(exec_path, X_OK) == 0) {
          // if it's found just break because exec_path is set
          break;
        }
        
        free(exec_path);
        exec_path = NULL;

        token = strtok(NULL, ":");
      }
      free(path_copy);
    }

    if (exec_path == NULL) {
      printf("Command '%s' not found\n", first_token);
      // TODO: check for internal commands (part 9)
    } else {
      pid_t pid = fork();
      if (pid == 0) {
        // child process

        // create pointer array for execv to have NULL at the end
        char** argv = malloc((tokens->size + 1) * sizeof(char*));
        for (int i = 0; i < num_tokens; i++) {
          argv[i] = tokens->items[i];
        }
        argv[num_tokens] = NULL;

        if (execv(exec_path, argv) == -1) {
          perror("execv failed");
        }
        free(argv);
        free(exec_path);
        // if execv returns, it failed
        _exit(EXIT_FAILURE);
      } else if (pid < 0) {
        // Fork failed
        perror("fork failed");
      } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        free(exec_path);
      }
    }

    for (int i = 0; i < tokens->size; i++) {
      if (DEBUG) printf("token %d: (%s)\n", i, tokens->items[i]);
    }

    free(input);
    free_tokens(tokens);
  }

  return 0;
}
