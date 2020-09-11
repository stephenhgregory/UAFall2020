#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 80 // The maximum length for a command

int main(void)
{
    char *args[MAX_LINE/2 + 1]; // Command-line arguments
    int should_run = 1; // Flag to determine when to exit the program

    while (should_run)
    {
        printf("osh> "); // Print command prompt
        fflush(stdout); // Flush the output buffer

        


        /**
         * After reading user input, the steps are:
         * (1) fork a child process using fork()
         * (2) the child process will invoke execvp()
         * (3) if command included &, parent will NOT invoke wait()
         */ 

    }

    return 0;
}