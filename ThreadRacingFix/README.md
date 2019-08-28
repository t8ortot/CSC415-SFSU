# Thread Racing Fix

This program is used as an example for how to fix the problems with the orginal ThreadRacing project by using mutex locks.

This program has been coded to utilize mutex locks to control thread access to the global shared value. When executed, the program will initialize ten threads of two types, adders and subtracters. These ten threads will do the action of their type to the global variable 25 times and return the current status each time. Once the threads are complete, the program will output the final value of the global variable, which should be 0.

Compile Instructions:
- gcc -I -Wall pthread_race.c -o threadracer -lpthread

Run Instructions:
- Once you've reached step 4 above, you may use the command "./threadracer" to run the program while in the project's directory in the terminal.