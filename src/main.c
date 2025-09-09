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

		// replace tokens in the format of $<ENV_VAR>
        for (int i = 0; i < num_tokens; i++) {
            if (tokens->items[i][0] == '$') {
                char* var_name = tokens->items[i] + 1;  // skip '$'
                char* var_value = getenv(var_name);
                if (var_value) {
                    free(tokens->items[i]);
                    char* new_token = malloc(strlen(var_value) + 1);
                    strcpy(new_token, var_value);
                    tokens->items[i] = new_token; // replace in the token list
                }
            }
        }

		for (int i = 0; i < tokens->size; i++) {
			printf("token %d: (%s)\n", i, tokens->items[i]);
		}

		free(input);
		free_tokens(tokens);
	}

	return 0;
}