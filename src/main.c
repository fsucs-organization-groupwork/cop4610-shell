#include "lexer.h"
#include <stdio.h>

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
		for (int i = 0; i < tokens->size; i++) {
			printf("token %d: (%s)\n", i, tokens->items[i]);
		}

		free(input);
		free_tokens(tokens);
	}

	return 0;
}