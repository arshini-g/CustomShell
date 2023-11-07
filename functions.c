#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_HISTORY 20
#define DEFAULT_DISPLAY_SIZE 10

#define HISTORY_FILE "command_history.txt"

#include <stdbool.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_TOKENS 64

bool quoted(const char *str)
{
    if ((str[0] == '"' && str[strlen(str) - 1] == '"') || (str[0] == '\'' && str[strlen(str) - 1] == '\'')) {
        return true;
    }
    return false;
}

char *remove_quotes(char *str)
{
    str[strlen(str) - 1] = '\0'; 
    return &str[1]; 
}

char *gone_space(const char *str)
{
    char *result = (char *)malloc(strlen(str) + 1);
    if (!result)
        return NULL;

    int i = 0, j = 0;
    bool prev_space = false;

    while (str[i])
    {
        if (isspace(str[i]))
        {
            if (!prev_space)
            {
                result[j++] = ' ';
                prev_space = true;
            }
        }
        else
        {
            result[j++] = str[i];
            prev_space = false;
        }
        i++;
    }
    result[j] = '\0';
    return result;
}

char history[MAX_HISTORY][MAX_INPUT_LENGTH];
int current_cmd_count = 0;

void load_history()
{
    FILE *file = fopen(HISTORY_FILE, "r");
    if (file)
    {
        int i = 0;
        while (fgets(history[i], MAX_INPUT_LENGTH, file))
        {
            char *newline = strchr(history[i], '\n');
            if (newline)
                *newline = '\0';
            i++;
            current_cmd_count++;
        }
        fclose(file);
    }
}

void save_history()
{
    FILE *file = fopen(HISTORY_FILE, "w");
    if (!file)
    {
        perror("Error saving command history");
        return;
    }
    int start = (current_cmd_count > MAX_HISTORY) ? (current_cmd_count - MAX_HISTORY) : 0;
    for (int i = start; i < current_cmd_count; i++)
    {
        fprintf(file, "%s\n", history[i % MAX_HISTORY]);
    }
    fclose(file);
}

void add_to_history(const char *command)
{
    if (current_cmd_count > 0 && strcmp(history[(current_cmd_count - 1) % MAX_HISTORY], command) == 0)
    {
        return;
    }

    strncpy(history[current_cmd_count % MAX_HISTORY], command, MAX_INPUT_LENGTH);
    current_cmd_count++;
    save_history();
}

void print_history()
{
    int start = (current_cmd_count > MAX_HISTORY) ? (current_cmd_count - MAX_HISTORY) : 0;
    int end = current_cmd_count;

    for (int i = end - 1; i >= start && i >= (end - DEFAULT_DISPLAY_SIZE); i--)
    {
        printf("%s\n", history[i % MAX_HISTORY]);
    }
}

void print_echo(char *tokens[], int num_tokens)
{
    for (int i = 1; i < num_tokens; i++)
    {
        char *arg = tokens[i];

        if (quoted(arg))
        {
            arg = remove_quotes(arg);
            printf("%s", arg);
        }
        else
        {
            char *cleaned = gone_space(arg);
            if (cleaned)
            {
                printf("%s", cleaned);
                free(cleaned);
            }
        }

        if (i < num_tokens - 1)
        {
            printf(" ");
        }
    }
    printf("\n");
}

void print_cd(char *tokens[], int num_tokens)
{
    char current_pwd[1024];
    if (getcwd(current_pwd, sizeof(current_pwd) - 1) == NULL)
    {
        perror("getcwd");
        return;
    }

    char *oldpwd = getenv("OLDPWD");

    if (num_tokens < 2)
    {
        char *home = getenv("HOME");

        if (chdir(home) == 0)
        {
            setenv("OLDPWD", current_pwd, 1);
            if (setenv("PWD", home, 1) == -1)
            {
                perror("setenv");
            }
        }
        else
        {
            perror("chdir");
        }
        return;
    }

    if (strcmp(tokens[1], "~") == 0)
    {
        char *home = getenv("HOME");

        if (chdir(home) == 0)
        {
            setenv("OLDPWD", current_pwd, 1);
            if (setenv("PWD", home, 1) == -1)
            {
                perror("setenv");
            }
        }
        else
        {
            perror("chdir");
        }
        return;
    }
    else if (strcmp(tokens[1], "-") == 0)
    {
        if (chdir(oldpwd) == 0)
        {
            char new_pwd[1024];
            if (getcwd(new_pwd, sizeof(new_pwd) - 1) == NULL)
            {
                perror("getcwd");
                return;
            }

            if (setenv("PWD", new_pwd, 1) == -1)
            {
                perror("setenv");
            }

            if (setenv("OLDPWD", current_pwd, 1) == -1)
            {
                perror("setenv");
            }
            printf("%s\n", new_pwd);
        }
        else
        {
            perror("chdir");
        }
    }

    else if (chdir(tokens[1]) == 0)
    {
        char new_pwd[1024];
        if (getcwd(new_pwd, sizeof(new_pwd) - 1) == NULL)
        {
            perror("getcwd");
            return;
        }

        if (setenv("OLDPWD", current_pwd, 1) == -1)
        {
            perror("setenv");
        }

        if (setenv("PWD", new_pwd, 1) == -1)
        {
            perror("setenv");
        }
    }
    else
    {
        perror("chdir");
    }
}

void disp()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        char *user = getenv("USER");
        char hostname[1024];

        gethostname(hostname, sizeof(hostname));
        char *currentDir = basename(cwd);

        printf("<%s@%s:%s> ", user, hostname, currentDir);
    }
    else
    {
        perror("getcwd");
    }
}

void read_input(char *input)
{
    fgets(input, MAX_INPUT_LENGTH, stdin);
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
    {
        input[len - 1] = '\0';
    }
}

int input_separator(char *input, char *tokens[], int *background)
{
    int num_tokens = 0;
    bool in_quote = false;
    char *start = input;
    char quote_char = '\0';

    for (size_t i = 0; i < strlen(input); i++)
    {
        if ((input[i] == '"' || input[i] == '\'') && !in_quote)
        {
            in_quote = true;
            start = &input[i] + 1;
            quote_char = input[i];
        }
        else if (input[i] == quote_char && in_quote)
        {
            in_quote = false;
            input[i] = '\0';
            tokens[num_tokens++] = start;
        }
        else if (isspace(input[i]) && !in_quote)
        {
            input[i] = '\0';
            if (start != &input[i])
            {
                tokens[num_tokens++] = start;
            }
            while (isspace(input[i + 1]))
                i++;
            start = &input[i + 1];
        }
        else if (!in_quote && i == 0)
        {
            start = &input[i];
        }
    }
    if (*start != '\0')
    {
        tokens[num_tokens++] = start;
    }
    return num_tokens;
}

int builtin_command(char *tokens[], int num_tokens)
{
    if (num_tokens > 0)
    {
        if (strcmp(tokens[0], "cd") == 0)
        {
            print_cd(tokens, num_tokens);
            return 1;
        }
        else if (strcmp(tokens[0], "exit") == 0)
        {
            exit(EXIT_SUCCESS);
        }
        else if (strcmp(tokens[0], "pwd") == 0)
        {
            print_pwd();
            return 1;
        }
    }
    return 0;
}

void print_pwd()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("pwd");
    }
}

