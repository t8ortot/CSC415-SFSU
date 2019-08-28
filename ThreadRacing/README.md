# Thread Racing

This program is used as an example for how thread racing conditions can occur when using mutliple threads.

How to build (assuming in ThreadRacing directory):
- gcc -I -Wall pthread_race.c -o threadracer -lpthread

How to run:
1. Once build, use the command "./threadracer" to start the program.
2. Once started, the program will instantiate an even number of adding and subtracting threads to operate on the global variable, which starts at 0.
3. This program will output all changes to the terminal and print a final value of the global variable. Given that there were the same number of threads of both type, the correct value for the global variable should be 0, however due to racing conditions the value is something different (in most trials, having a value of 0 is either a coincidence, or the operating system is not busy enough with other processes to pause any of the threads).