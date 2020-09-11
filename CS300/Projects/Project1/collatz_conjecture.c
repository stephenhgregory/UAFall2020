#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>

void printCollatzSequence(int *n)
{
    while (*n > 1)
    {
        printf("%d, ", *n);

        // If n is even...
        if (*n % 2 == 0)
            *n /= 2;

        // Else if n is odd..
        else
            *n = (3 * *n) + 1;
    }

    printf("%d\n", *n);
}

void printCommandLineArguments(int* argc, char** argv) 
{
    if (*argc <= 1)
    {
        return;
    }

    printf("You have entered %d arguments:\n", *argc-1);
    for (int i = 1; i < *argc-1; i++)
    {
        printf("%s, ", argv[i]);
    }
    printf("%s\n", argv[(*argc)-1]);
}

void childProcess(int* startNumber)
{
    printf("Currently inside of the childProcess function...\n");
    printCollatzSequence(startNumber);
    printf("Child is exiting...\n");
}

void parentProcess(int* status)
{
    printf("Parent is waiting...\n");
    wait(status);
    printf("Status returned from child: %d\n", *status);
    printf("Parent is exiting...\n");
}

int main(int argc, char **argv)
{
    pid_t forkStatus;
    int status;
    int startNumber;

    // If the passed-in integer isn't a positive integer, exit the program
    if (argc < 2 || atoi(argv[1]) <= 0)
    {
        printf("Passed-in argument is not a positive integer. Try again, like so:\n./collatz_conjecture <insert_int>\n");
        exit(0);
    }

    // Get the starting number for the Collatz Conjecture
    startNumber = atoi(argv[1]);

    // Create the child process
    forkStatus = fork();

    // Child process
    if (forkStatus == 0)
    {
        childProcess(&startNumber);
    }

    // Parent Process
    else if (forkStatus > 0)
    {
        parentProcess(&status);
    }

    else
    {
        perror("There was an error calling the fork() function.\n");
    }

}