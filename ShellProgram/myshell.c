/****************************************************************
 * Name        : James Clark                                    *
 * Date        : 6/26/18                                        *
 * Description :  Writting a simple bash shell program          *
 * 	              that will execute simple commands. The main   *
 *                goal of the assignment is working with        *
 *                fork, pipes and exec system calls.            *
 ****************************************************************/
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

//Max amount allowed to read from input
#define BUFFERSIZE 256
//max ampunt of args
#define ARGVMAX 32
//max size of args
#define ARGSIZEMAX 32

int main(int argc, char **argv) {

    //holds args
    char *myargv[ARGVMAX];
    //buffer to tokenize
    char *buffer = malloc(BUFFERSIZE);
    //tokens to be added to myargv
    char *token;
    // size of myargv
    int myargc = 0;
    //delimiters
    char *delim = " \t\n";
    //wait status
    int wstatus;
    //true if appending file
    int aFile = 0;
    //true if overwriting file
    int oFile = 0;
    //true if changing directory
    int cd = 0;
    //true if getting current working directory
    int pwd = 0;
    //true if piping
    int pip = 0;
    //true if pipe output is ready to be read
    int pipread = 0;
    //EC: new modifiable prompt
    char *prompt = "myshell";
    //name of output file
    char *outputName = 0;
    //output file
    int outputFile = 0;
    //universal pipe
    int fileD[2];

    for (int i = 0; i < ARGVMAX; i++) {
        myargv[i] = calloc(ARGSIZEMAX, sizeof(char));
    }

    while (1) {
        //waits for background processes
        if (waitpid(-1, &wstatus, WNOHANG | WUNTRACED) == 0) {}

        //EC: gets current directory
        char directory[2000];
        getcwd(directory, sizeof(directory));

        //print prompt
        printf("%s ~%s >> ", prompt, directory);

        //get input
        fgets(buffer, BUFFERSIZE, stdin);

        //if only enter was hit, skip
        if (strcmp(buffer, "\n") != 0) {

            token = strtok(buffer, delim);

            while (token != NULL) {

                if (strcmp(token, "exit") == 0) {
                    while (1) {
                        return 916;
                    }
                }

                myargv[myargc] = token;
                myargc++;

                //if waiting for an output file name
                if ((oFile || aFile) && !outputName) {
                    outputName = token;
                    myargv[myargc - 1] = NULL;
                    myargc--;
                }

                //all special character cases
                if (strcmp(token, ">") == 0) {
                    oFile = 1;
                    myargv[myargc - 1] = NULL;
                    myargc--;

                } else if (strcmp(token, ">>") == 0) {
                    aFile = 1;
                    myargv[myargc - 1] = NULL;
                    myargc--;

                } else if (strcmp(token, "<") == 0) {
                    myargv[myargc - 1] = NULL;
                    myargc--;

                } else if (strcmp(token, "cd") == 0) {
                    cd = 1;
                    myargv[myargc - 1] = NULL;
                    myargc--;

                } else if (strcmp(token, "pwd") == 0) {
                    pwd = 1;
                    myargv[myargc - 1] = NULL;
                    myargc--;

                } else if (strcmp(token, "|") == 0) {
                    pip = 1;
                    myargv[myargc - 1] = NULL;
                    myargc--;
                    //crates a temporary file to store output of first command
                    outputFile = open("temp", O_WRONLY | O_CREAT | O_TRUNC, 0600);
                    //goes to CONTINUE to execute the command
                    goto CONTINUE;
                }

                //if piping, program will return here after the previous command has been completed
                NEXT:
                token = strtok(NULL, delim);
            }


            CONTINUE:
            if (cd) {
                if(myargc == 0){
                    chdir("/");
                } else if(chdir(myargv[myargc - 1]) == 0) {
                    myargv[myargc - 1] = NULL;
                    myargc--;
                } else {
                    myargv[myargc - 1] = NULL;
                    myargc--;s
                    perror("chrdir");
                }
            }

            //if piping, the temporary file name is appended to the next command for it to read
            if (pipread) {
                myargv[myargc] = "temp";
                myargc++;
                pipread = 0;
            }

            if (pwd) {
                printf("%s\n", directory);
            }

            if (oFile) {
                outputFile = open(outputName, O_WRONLY | O_CREAT | O_TRUNC, 0600);
            } else if (aFile) {
                outputFile = open(outputName, O_RDWR | O_CREAT | O_APPEND, 0600);
            }

            //NULL terminate myargv for execvp
            myargv[myargc] = NULL;

            //if there are arguments available
            if (myargc > 0) {
                int background;

                //if it is a background process
                if (strcmp(myargv[myargc - 1], "&") == 0) {
                    background = 1;
                    myargv[myargc - 1] = NULL;
                    myargc--;
                } else {
                    background = 0;
                }


                if (!background) {
                    if (outputFile || pip) {
                        //pipes fileD
                        if (pipe(fileD) == -1) {
                            perror("pipe");
                            exit(1);
                        }
                    }

                    //fork
                    pid_t ID = fork();

                    //if child process
                    if (ID == 0) {
                        if (outputFile || pip) {
                            close(outputFile);
                            //dup STDOUT
                            while ((dup2(fileD[1], STDOUT_FILENO) == -1)) {}
                            close(fileD[1]);
                            close(fileD[0]);
                        }

                        //execute command
                        if (execvp(myargv[0], myargv) == -1) {
                            perror("execvp error");
                        }

                        //if parent
                    } else {

                        do {
                            //if outputting or piping, save output of child process
                            if (outputFile || pip) {
                                char *outBuff = malloc(300);
                                ssize_t count = sizeof(outBuff);
                                while (count == sizeof(outBuff)) {
                                    count = read(fileD[0], outBuff, sizeof(outBuff));

                                    if (count == 0) {
                                        break;
                                    } else {
                                        write(outputFile, outBuff, (size_t) count);
                                    }
                                }
                                //tidy up
                                close(fileD[0]);
                                close(outputFile);
                                outputFile = 0;
                                aFile = 0;
                                oFile = 0;
                                outputName = 0;
                                free(outBuff);

                                //wait for child
                                waitpid(ID, &wstatus, WUNTRACED);
                            } else {
                                //wait for child
                                waitpid(ID, &wstatus, WUNTRACED);
                            }

                        } while (!WIFEXITED(wstatus));
                    }
                    //tidy up
                    if (outputFile || pip) {
                        close(fileD[1]);
                    }

                    //if background process
                } else {

                    pid_t ID = fork();

                    if (ID == 0) {

                        if (execvp(myargv[0], myargv) == -1) {
                            perror("execvp error");
                        }
                    }

                }
            }
        }

        //reset args
        for (int i = 0; i < ARGVMAX; i++) {
            myargv[i] = NULL;
        }
        myargc = 0;

        //if piping, pipe is ready to read, goes back into token loop
        if (pip) {
            pipread = 1;
            pip = 0;
            goto NEXT;
        }

        //tidy up
        free(buffer);
        buffer = malloc(BUFFERSIZE);
        cd = 0;
        pwd = 0;
        remove("temp");

    }
}