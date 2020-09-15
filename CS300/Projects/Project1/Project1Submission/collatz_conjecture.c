/**
 * Author: Stephen Gregory
 * CWID: 11812663
 * 
 * This program takes as input an integer, and prints the sequence described in the Collatz Conjecture.
 * This conjecture states that any integer will reach the integer value of 1 after executing the following
 * recursive sequence: If the number is even, divide it by 2. Else, if the number is odd, multiply it by 
 * 3 and add 1 to the result. When applied upon itself in a recursive fashion, the Collatz Conjecture states
 * that this sequence will arrive at the integer 1.
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>

void printCollatzSequence(long long *n)
{
    while (*n > 1)
    {
        printf("%lld, ", *n);

        // If n is even...
        if (*n % 2 == 0)
            *n /= 2;

        // Else if n is odd..
        else
            *n = (3 * *n) + 1;
    }

    printf("%lld\n", *n);
}

/**
 * Debugging function
**/
void printCommandLineArguments(int *argc, char **argv) 
{
    int i;
    if (*argc <= 1)
    {
        return;
    }

    printf("You have entered %d arguments:\n", *argc-1);
    for (i = 1; i < *argc-1; i++)
    {
        printf("%s, ", argv[i]);
    }
    printf("%s\n", argv[(*argc)-1]);
}

void childProcess(long long* startNumber)
{
    printCollatzSequence(startNumber);
}

void parentProcess(int* status)
{
    wait(status);
}

int main(int argc, char **argv)
{
    pid_t forkResult;
    int status;
    long long startNumber;

    // If the passed-in integer isn't a positive integer, exit the program
    if (argc < 2 || atoll(argv[1]) <= 0)
    {
        printf("Passed-in argument is not a positive integer. Try again, like so:\n./collatz_conjecture <insert_int>\n");
        exit(0);
    }

    // Get the starting number for the Collatz Conjecture
    startNumber = atoll(argv[1]);

    // Create the child process
    forkResult = fork();

    // Child process
    if (forkResult == 0)
    {
        childProcess(&startNumber);
    }

    // Parent Process
    else if (forkResult > 0)
    {
        parentProcess(&status);
    }

    else
    {
        perror("There was an error calling the fork() function.\n");
    }
    return 0;
}