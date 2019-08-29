/**
* A program to instantiate producer that creat objects the will be consumed by the consumer threads from a buffer.
* @ James Clark
*/
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
//added for O_CREAT
#include <fcntl.h>

#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define WHITE  "\x1B[37m"

//bufferItem type
typedef int bufferItem;

//sleep times
unsigned Ptime, Ctime;
//number of buffers, number of producers, number of consumers, number of items per producer
int N, P, C, X;
//buffer trackers, remainder value from (P*X) % C
int in, out, counter, remains;
//Final check array indexes
int pArrayIndex, cArrayIndex;
//Arrays of producer and consumer activity
int *producerArray, *consumerArray;
int perConsumer;
bufferItem item;
bufferItem *buffer;
//mutex lock
pthread_mutex_t lock;
//semaphores
sem_t *full, *empty;

/* 
 * Function to remove item.
 * Item removed is returned
 */
int dequeue_item(bufferItem *itemConsumed) {
 
    if (counter != 0) {

        //removes item from buffer and stores it in pointer
        *itemConsumed = buffer[out];
        out = (out + 1) % N;
        counter--;

        return *itemConsumed;
    } else {

        return -1;
    }
}

/* 
 * Function to add item.
 * Item added is returned.
 * It is up to you to determine
 * how to use the ruturn value.
 * If you decide to not use it, then ignore
 * the return value, do not change the
 * return type to void. 
 */
int enqueue_item(int item) {

    if (counter < N) {

        //inserts item into buffer
        buffer[in] = item;
        in = (in + 1) % N;
        counter++;
        return item;
    } else {

        return -1;
    }
}

void *producer(void *ID) {

    //assign ID
    int prodID = (int) ID;
    int numProduced = 0;

    while (numProduced < X) {

        numProduced++;
        sem_wait(empty);
        pthread_mutex_lock(&lock);
        item++;

        if (enqueue_item(item) == -1) {

            fprintf(stderr, RED "Insert error!");

        } else {

            printf(GREEN "%d was produced by producer-> %d\n", item, prodID);
            producerArray[pArrayIndex] = item;
            pArrayIndex++;
            sleep(Ptime);
        }

        pthread_mutex_unlock(&lock);
        sem_post(full);

    }

    return 0;
}

void *consumer(void *ID) {

    int conID = (int) ID;
    bufferItem itemConsumed;
    int num = 0;
    int toConsume = perConsumer;

    //handles over consume
    if (conID < remains) {
        toConsume++;
    }

    while (num < toConsume) {

        sem_wait(full);
        pthread_mutex_lock(&lock);

        if (dequeue_item(&itemConsumed) == -1) {

            fprintf(stderr, RED "Removal error!");
        } else {

            printf(RED "%d was consumed by consumer-> %d\n", itemConsumed, conID);
            consumerArray[cArrayIndex] = itemConsumed;
            cArrayIndex++;
            sleep(Ctime);
        }

        num++;
        pthread_mutex_unlock(&lock);
        sem_post(empty);
    }

    return 0;
}


/*
 * compares the producer and consumer arrays to verify the program functions properly
 */
int compareArrays() {

    for (int i = 0; i < (P * X); i++) {
        if (producerArray[i] != consumerArray[i]) {
            return 0;
        }
    }
    return 1;
}


int main(int argc, char **argv) {

    //arg check
    if (argc != 7) {
        printf(RED "6 arguments required! (N, P, C, X, Ptime, Ctime)\n");
        exit(-1);
    }

    //Buffer size
    N = atoi(argv[1]);
    //Number of producers
    P = atoi(argv[2]);
    //Number of consumers
    C = atoi(argv[3]);
    //Number of items that will be produced
    X = atoi(argv[4]);
    //Producer sleep time
    Ptime = atoi(argv[5]);
    //Consumer sleep time
    Ctime = atoi(argv[6]);

    //Allocate memory
    buffer = (int *) malloc(sizeof(int) * N);
    producerArray = malloc(sizeof(int) * P * X);
    consumerArray = malloc(sizeof(int) * P * X);

    //initialize buffer trackers
    in = out = counter = 0;
    //compute remainder
    remains = (P * X) % C;
    //items per consumer
    perConsumer = ((P * X) / C);

    //initialize mutex lock
    pthread_mutex_init(&lock, NULL);

    sem_unlink("/empty_sem");
    sem_unlink("/full_sem");

    empty = sem_open("/emptySem", O_CREAT, 0777, N);

    if (empty == SEM_FAILED) {
        perror(RED "Failed to open empty semaphore");
        exit(-1);
    }

    full = sem_open("/fullSem", O_CREAT, 0777, 0);

    if (full == SEM_FAILED) {
        perror(RED "Failed to open empty semaphore");
        exit(-1);
    }


    //starting prompt
    time_t start;
    start = time(NULL);
    printf(WHITE "\nCurrent time: %s\n", ctime(&start));
    printf("Number of Buffers : %d\n", N);
    printf(GREEN "Number of Producer : %d\n", P);
    printf(RED "Number of Consumers : %d\n", C);
    printf(GREEN "Number of items Produced by each producer : %d\n", X);
    printf(RED "Number of items consumed by each consumer : %d\n", perConsumer);
    printf(WHITE "Over consume on? : ");
    if(remains == 0){
        printf("no\n");
    } else {
        printf("yes\n");
    }
    printf("Over consumed amount : %d\n", remains);
    printf(GREEN "Time each Producer Sleeps (seconds) : %d\n", Ptime);
    printf(RED "Time each Consumer Sleeps (seconds) : %d\n\n", Ctime);

    //array of producer threads
    pthread_t producers[P];
    //create P amount of threads
    for (int i = 0; i < P; i++) {
        pthread_create(&producers[i], NULL, producer, (void *) i);
    }

    //array of consumer threads
    pthread_t consumers[C];
    //create C amount of threads
    for (int i = 0; i < C; i++) {
        pthread_create(&consumers[i], NULL, consumer, (void *) i);
    }

    //join all producer threads
    for (int i = 0; i < P; i++) {
        pthread_join(producers[i], NULL);
        printf(GREEN "Producer Thread joined: %d\n", i);
    }

    //join all consumer threads
    for (int i = 0; i < C; i++) {
        pthread_join(consumers[i], NULL);
        printf(RED "Consumer Thread joined: %d\n", i);
    }

    // finish time
    time_t finish;
    finish = time(NULL);
    int finalTime = (int) finish - (int) start;
    printf(WHITE "\nCurrent time: %s\n", ctime(&start));



    //prints out arrays
    fprintf(stdout, "%-10s%-2s%s\n", "Producers", "|", "Consumers");
    for (int i = 0; i < (P * X); i++) {
        fprintf(stdout, "%-10d%-2s%d\n", producerArray[i], "|", consumerArray[i]);
    }

    //compares arrays
    if (compareArrays() == 1) {
        printf(GREEN "\nProducer and Consumer Arrays Match!");
    } else if (compareArrays() == 0) {
        printf(RED "\nProducer and Consumer Arrays Do Not Match!");
    }

    printf(WHITE "\n\nTotal Runtime: %d secs\n", finalTime);

}
