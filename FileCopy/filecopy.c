/**
* A file copying program that copies an input file to a specified output destination.
* @author James Clark
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

extern int errno;

// Used when creating your buffer and also to limit the amount each read CAN do.
#define BUFF_MAX 13

int main(int argc, char const *argv[]) {

    char *buffer = malloc(BUFF_MAX);
    char *input = malloc(100 * sizeof(char));
    char *output = malloc(100 * sizeof(char));
    int inputFile;
    int outputFile;

    printf("Welcome to the File Copy Program by James Clark!\n");

    // promtps for file name to copy from
    printf("Enter the name of the file to copy from:\n");
    scanf("%s", input);
    inputFile = open(input, O_RDONLY);

    // prompts for name a file to copy to
    printf("Enter the name of the file to copy to:\n");
    scanf("%s", output);
    outputFile = open(output, O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    // if the input file does exist
    if (inputFile != -1) {
        // and if the output file name already exists
        if (outputFile != -1) {

            ssize_t reading;
            int buffNum = 0;
            printf("\n");

            // copy input to buffer and then export to output destination
            while ((reading = read(inputFile, buffer, BUFF_MAX)) != 0) {
                buffNum++;
                printf("Buffer Iteration %d: %s\n", buffNum, buffer);
                write(outputFile, buffer, (size_t) reading);
                bzero(buffer, BUFF_MAX);
            }


        } else {
            printf("Output file exists already!");
        }
    } else {
        printf("Input file does not exist!");
    }

    free(input);
    free(output);
    close(inputFile);
    close(outputFile);

    return 0;
}