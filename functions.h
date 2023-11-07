#ifndef FUNCTIONS_H
#define FUNCTIONS_H
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

#define MAX_HISTORY_SIZE 20
#define DEFAULT_DISPLAY_SIZE 10

#include <stdbool.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_TOKENS 64

void disp();
void read_input(char *input);
int input_separator(char *input, char *tokens[], int *background);

void print_echo(char *tokens[], int num_tokens);
int builtin_command(char *tokens[], int num_tokens);
//void execute_command(char *tokens[], int num_tokens, int background);
void print_pwd();
void print_cd(char *tokens[], int num_tokens);

void load_history();
void save_history();
void add_to_history(const char *command);
void print_history();

#endif
