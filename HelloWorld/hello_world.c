/**
 * A Hello World program that prints without uding the printf() function.
 * @author James Clark
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define NAME "James Clark"

int main(int argc,  char** argv)
{
	int len;
	ssize_t bytes_written;
	char* out = malloc(64);
	len = sprintf(out, "Hello World! This program written by %s!!!\n", NAME);

	if(!len){
		perror("error in sprintf");
	}

	bytes_written = write(1, out, len);

	if (bytes_written < 0){
		perror("error in write");
	}

	free(out);
	return 0;
}
