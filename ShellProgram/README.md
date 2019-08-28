# Shell Program

This program is a simple shell program that mimics the basic behaviors of the linux shell.

# Build Instructions:
- gcc -I -Wall myshell.c -o myshell

# Run Instructions:
1. Once built, you may use the command "./myshell" to run the program while in the project's directory in the terminal.
2. You will be given a prompt, similar to the terminal.
3. Enter a command into the program. The following commands and most of their flags have been tested and work:
	- ls
	- cat
	- ls
	- cd
	- pwd
	- background processes using &
	- redirect standard output using > or >>
	- redirect standard input from a file using <
	- piping multiple commands using |

4. The shell will execute the command given, or give an error for incorrect syntax.
5. Repeat steps 3 thru 5...
6. If you want to exit the shell, either use the normal terminal exit command ^C, or type "exit".