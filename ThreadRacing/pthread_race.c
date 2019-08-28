/**
* A program used as an example for problems with thread racing when using multiple threads.
* @author James Clark
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define MAX_THREADS 10
#define MAX_ITERATIONS 25


/**
 * this struct is used as a parameter for the function nanosleep.
 * For exmaple : nanosleep(&ts, NULL);
 */
struct timespec ts = {2 , 0};

// global variable to be changed by threads.
int globVar = 0;
int thread = 0;

// adder function for thread to add 1 to the global variable
void *adder(void * num){

    int* temp;
    int tid = thread;
    thread++;

    for(int i = 0; i < MAX_ITERATIONS; i++) {

        temp = num;
        *temp = *temp + 1;
        nanosleep(&ts, NULL);
        globVar = *temp;

        printf("Current Value written to Global Variables by thread : %d is %d\n", tid, *temp);

    }
    return 0;
}

// subtracter to remove 1 from the global variable
void *subber(void * num){
    
    int* temp;
    int tid = thread;
    thread++;

    for (int i = 0; i < MAX_ITERATIONS; i++) {

        temp = num;
        nanosleep(&ts, NULL);
        *temp = *temp - 1;
        globVar = *temp;

        printf("Current Value written to Global Variables by thread : %d is %d\n", tid, *temp);

    }
    return 0;
}



int main(int argc, char** argv) {

    pthread_t threads[MAX_THREADS];

    // create MAX_THREADS amount of threads
    for(int i = 0; i < MAX_THREADS; i++){
        pthread_t pthread;

        // make half into adders and half subbers
        if ((i % 2 == 0) || (i==0)) {
            pthread_create(&pthread, NULL, adder,  &globVar);
        } else {
            pthread_create(&pthread, NULL, subber, &globVar);
        }

        threads[i] = pthread;
    }

    // join threads
    for(int i = 0; i < MAX_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    // print final value.
    printf("Final Value of Shared Variable : %d\n", globVar);
    return 0;
}
