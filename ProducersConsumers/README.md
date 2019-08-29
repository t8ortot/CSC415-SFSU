# Producer and Consumer Threads

This program creates two different types of threads: Producers and Consumers. The Producers will add objects into a buffer and consumers will remove those items from the buffer. At the end of the program, it is verified that the Consumer threads reteives all the objects created by the Producer threads from the buffer.

Compile Instructions:
- gcc -I -Wall pandc.c -o pandc -lpthread

Run Instructions:

1. Once built, you may use the command "./pandc", followed by 6 integer values (Number of Buffers, Number of Producers, Number of Consumers, Number of items to be Produced per thread, Sleep time for Producers, Sleep time for Consumers) to run the program while in the project's directory in the terminal.
2. The program will read the arguments given and spawn an N-sized buffer, P amount of Producers, and C amount of Consumers. The Producer threads will each enqueue X amount of items into the buffer. The Consumer threads will dequeue all items from the buffer. Both the Producers and Consumers will sleep for a given time, specified as Ptime and Ctime.
3. When the threads are complete, the Producer and Consumer Arrays are compared. If the arrays are found to be the same, the program functioned properly.
