# File Copy

This program will copy a specified file and create a new file with a given name with the same contents as the input file.

# Build Instructions (assuming in FileCopy directory):
- gcc -I -Wall filecopy.c -o fc

# Run Instructions:

1. Once built, you may use the command "./fc" to run the program while in the project's directory in the terminal.
2. You will be prompted to enter the name of a file to copy. You make copy any type of file, given the correct path. For ease, you can type "readme.txt" to copy the .txt file in this directory.
3. You will be prompted to enter the name of the file to be created and copied to. To ensure a matching output, make sure to specify the same file extension as input.
4. If the input file does not exist or the output file already exists, the program will end itself. Otherwise, the program will copy the input file to the output file, display the buffer output, and end.