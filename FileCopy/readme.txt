File Copy

This program will copy a specified file and create a new file with a given name with the same contents as the input file.

Build Instructions (assuming in FileCopy directory):
- gcc -I -Wall filecopy.c -o fc

Run Instructions:

1. Once built, you may use the command "./fc" to run the program while in the project's directory in the terminal.
2. You will be prompted to enter the name of a file to copy.
3. You will be prompted to enter the name of the file to be created and copied to.
4. If the input file does not exist or the output file already exists, the program will end itself. Otherwise, the program will copy the input file to the output file, display the buffer output, and end.

strace output:
% time     seconds  usecs/call     calls    errors syscall     annotations
------ ----------- ----------- --------- --------- ----------- ------------
  0.00    0.000000           0         9           read        read is called once in initialization of program, twice for user inputs, and 6 times for reading the input file during copy process.
  0.00    0.000000           0        17           write       write is called 17 times for every output to the terminal.
  0.00    0.000000           0         4           open        open is called in initialization of program twice, once for opening the input file, and once for creating and opening output file.
  0.00    0.000000           0         4           close       close is called twice in initialization of program and twice at the end of the program to close open files.
  0.00    0.000000           0         4           fstat       fstat is called four times in the initialization of the program.
  0.00    0.000000           0         1         1 lseek       lseek is called at the end of the program
  0.00    0.000000           0         8           mmap        mmap is called 8 times in initialization of program.
  0.00    0.000000           0         4           mprotect    mprotect is called once in initialization of program, and three times to create pointers in program
  0.00    0.000000           0         1           munmap      munmap is called after creation of program's pointers
  0.00    0.000000           0         3           brk         brk is called once in initialization of program and twice in variable creations
  0.00    0.000000           0         3         3 access      access is called three times in initialization of program
  0.00    0.000000           0         1           execve      execve is the first syscall made in the program
  0.00    0.000000           0         1           arch_prctl  arch_prctl is called once in initialization of program
------ ----------- ----------- --------- --------- ----------- -------------
100.00    0.000000                    60         4 total