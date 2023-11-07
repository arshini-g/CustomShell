# main.c

This C code serves as the main program for a basic command-line shell. It provides a simple and interactive environment for users to enter and execute commands. The shell supports both built-in commands and external commands. Additionally, it maintains a command history feature for tracking previously entered commands.

## Key Features

   - The code includes standard C libraries such as `stdio.h`, `stdlib.h`, `string.h`, `unistd.h`, `sys/types.h`, and `sys/wait.h`.
   - It also includes a custom header file named "functions.h," which contains function declarations and custom-defined constants.
   - The shell runs in an infinite loop, continuously accepting and processing user input.
   - The shell loads and maintains a command history, allowing users to access previously executed commands.
   - It recognizes and executes built-in commands like "echo" and "history."
   - The shell can execute external commands entered by users, forking new processes to run them.

# functions.h

The "functions.h" header file is an integral part as it provides function prototypes, constants, and data structures for core functionalities such as command execution, input processing, and command history management. This header file is included in the main program to build a custom command-line interface. 

# functions.c

These functions handle tasks such as parsing user input, executing commands, managing command history, and processing built-in commands.

## Key Functions and Features

- **Quoted String Detection:** The `quoted` function checks whether a string is enclosed in single or double quotes.

- **Removing Quotes:** The `remove_quotes` function removes enclosing quotes from a string.

- **Whitespace Removal:** The `gone_space` function removes extra whitespace characters from a string while preserving a single space.

- **Command History:** The code keeps track of previously entered commands and provides functions to load, save, and display command history.

- **Built-In Command Execution:** The `builtin_command` function checks for and executes built-in commands like "cd," "exit," and "pwd."

- **Echo Command Handling:** The `print_echo` function processes and prints the arguments of the "echo" command, supporting quoted strings and whitespace removal.

- **Change Directory (cd) Handling:** The `print_cd` function manages changing the current working directory, including handling special paths like "~" and "-".

- **Display Function:** The `disp` function displays a customized command prompt with user, hostname, and current directory information.

- **User Input Reading:** The `read_input` function reads user input from the standard input.

- **Input Tokenization:** The `input_separator` function tokenizes user input into an array of tokens, taking into account quoted strings and whitespace.

# Usage

To use this command-line shell:

1. Compile the code.
2. Run the compiled program.
3. You will be presented with a command prompt where you can enter various commands.
4. You can enter echo, pwd, cd and history commands like you do in your own system shell. 
5. The shell will recognize and execute built-in commands and handle external commands by forking processes.
6. The command history is maintained and can be displayed using the "history" command.
7. Run "make" in your terminal to get into the shell or compile and run like you would any C program by linking all the files.
