/**
* A program used as an example for solving problems with thread racing when using multiple threads by using mutex locks.
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
 * This struct is used as a parameter for the function nanosleep.
 * For exmaple : nanosleep(&ts, NULL);
 */
struct timespec ts = {0 , 30};

// global variable to be changed by threads
int globVar = 0;
int thread = 0;
// mutex lock to allow access to one thread at a time
pthread_mutex_t lock;

// adder function for thread to add 1 to the global variable
void *adder(void * num){
    int* temp;

    int tid = thread;
    thread++;

    for(int i = 0; i < MAX_ITERATIONS; i++) {

    	pthread_mutex_lock(&lock);
        temp = num;
        *temp = *temp + 1;
        nanosleep(&ts, NULL);
        globVar = *temp;
        pthread_mutex_unlock(&lock);

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

    	pthread_mutex_lock(&lock);
        temp = num;
        nanosleep(&ts, NULL);
        *temp = *temp - 1;
        globVar = *temp;
 		pthread_mutex_unlock(&lock);

        printf("Current Value written to Global Variables by thread : %d is %d\n", tid, *temp);

    }
    return 0;
}


int main(int argc, char** argv) {

	 if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        return 1;
    }

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

    pthread_mutex_destroy(&lock);

    // print final value.
    printf("Final Value of Shared Variable : %d\n", globVar);
    return 0;
}
