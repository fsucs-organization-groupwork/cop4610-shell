#include "lexer.h"
#include <stdio.h>
#include <string.h>

int main()
{
	while (1) {
		// get $USER, $MACHINE, $PWD env variables
        char *user = getenv("USER");
        char *machine = getenv("MACHINE");
        char *pwd = getenv("PWD");

		printf("%s@%s:%s> ", user, machine, pwd);

		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */

		char *input = get_input();
		printf("whole input: %s\n", input);

		tokenlist *tokens = get_tokens(input);
		int num_tokens = tokens->size;

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

		// search all paths in $PATH env variable for executable files in tokens[0]
		const char* path = getenv("PATH");
        char* path_copy = malloc(strlen(path) + 1);
        strcpy(path_copy, path);

        char* token = strtok(path_copy, ":");
        while (token != NULL) {
			printf("PATH token: %s\n", token);
            token = strtok(NULL, ":");

			// TODO: check if executable exists in this path for tokens[0]
        }

		free(path_copy);

		for (int i = 0; i < tokens->size; i++) {
			printf("token %d: (%s)\n", i, tokens->items[i]);
		}

		free(input);
		free_tokens(tokens);
	}

	return 0;
}