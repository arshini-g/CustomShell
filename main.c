#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "functions.h"

int main()
{
    char input[MAX_INPUT_LENGTH];
    char *tokens[MAX_TOKENS];
    load_history();

    while (1)
    {
        disp();
        read_input(input);

        add_to_history(input);

        int num_tokens = input_separator(input, tokens, NULL);

        if (num_tokens > 0)
        {
            if (strcmp(tokens[0], "echo") == 0)
            {
                print_echo(tokens, num_tokens);
            }
            else if (strcmp(tokens[0], "history") == 0)
            {
                print_history();
            }
            else if (builtin_command(tokens, num_tokens) == 1)
            {
                continue;
            }
            else
            {
                pid_t pid = fork();
                if (pid == -1)
                {
                    perror("Fork failed");
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {   
                    perror("Child process failed");
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    waitpid(pid, NULL, 0);
                }
            }
        }
    }

    return 0;
}
