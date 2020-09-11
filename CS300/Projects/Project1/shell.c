#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/wait.h>

#define MAX_LINE 80 // The maximum length for a command
#define COMMAND_HISTORY_LEN 10 // The length of the command history

void resetVars(int *ampersandSeen, int *noCommandRun)
{
    *ampersandSeen = 0;
    *noCommandRun = 0;
}

void executeLastCommand(char rawArgsHistory[COMMAND_HISTORY_LEN][MAX_LINE], char *args[], int index)
{
    if (index - 1 <= 0 )
    {
        // TODO: Handle error
        return;
    }

    tokenizeArgs(rawArgsHistory, args, char *rawArgs, index, ampersandSeen, noCommandRun) 


}

void showCommandHistory(char rawArgsHistory[COMMAND_HISTORY_LEN][MAX_LINE], int index)
{
    for (int i = index - 1; i > (index - COMMAND_HISTORY_LEN - 1) && i > 0; i--)
    {
        if (rawArgsHistory[i] != NULL)
            printf("%d %s\n", i, rawArgsHistory[i%10]);
    }
}

void executeNthCommand(char rawArgsHistory[COMMAND_HISTORY_LEN][MAX_LINE], char *args[], int N, int index)
{

}

void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

void copyRawArgs(char *rawArgsCopy, char *rawArgs)
{
    strcpy(rawArgsCopy, rawArgs); // Copy the contents of rawArgs to rawArgsCopy
    int len = strlen(rawArgsCopy);
    if (len > 0 && rawArgsCopy[len-1] == '\n') // Remove trailing \n
        rawArgsCopy[--len] = '\0';
}

void tokenizeArgs(char rawArgsHistory[COMMAND_HISTORY_LEN][MAX_LINE], char *args[], char *rawArgs, int index, int *ampersandSeen, int *noCommandRun)
{
    int i = 0; // Counter
    char rawArgsCopy[MAX_LINE]; // Copy of the raw argument string

    copyRawArgs(rawArgsCopy, rawArgs); // Copy the rawArgs string to rawArgsCopy

    args[i] = strtok(rawArgs, " \n"); // Get the first token

    if (args[i] == NULL) // Handles special case where command is NULL (no command)
    {
        *noCommandRun = 1;
        return;
    }

    else if (strcmp(args[i], "history") == 0) // Handle special case of "history" command
    {
        if (index - 1 <= 0)
        {
            // TODO: Handle error
        }
        // Recursively call tokenizeArgs with the previous command from rawArgsHistory
        tokenizeArgs(rawArgsHistory, args, rawArgsHistory[(index-1)%COMMAND_HISTORY_LEN], index, ampersandSeen, noCommandRun);
    }

    else if (strcmp(args[i], "!!") == 0) // Handle special case of "!!" command
    {
        showCommandHistory(rawArgsHistory, index); // List the command history
        *noCommandRun = 1;
        return;
    }
    
    else if (args[i][0] == '!') // Handle special case of "!N" command
    {
        removeChar(args[i], '!'); // Remove the '!' from the command
        int N = atoi(args[i]); // Get the command # to execute
        executeNthCommand(rawArgsHistory, args, N, index); // Execute the Nth command
        *noCommandRun = 1;
        return;
    }

    while (args[i] != NULL) // Get each next token
    {
        if (strcmp(args[i], "&") == 0) // If one of the tokens is "&"
        {
            *ampersandSeen = 1; // Set the flag for "ampersand Seen"
            i--; // Then decrement so that we don't count the & as an argument, by overwriting it in the array
        }
        i++;
        args[i] = strtok(NULL, " \n"); // Get the next token from Args
    }
    
    printf("Copying %s to rawArgsHistory[%d]\n", rawArgsCopy, index%COMMAND_HISTORY_LEN);
    strcpy(rawArgsHistory[index%COMMAND_HISTORY_LEN], rawArgsCopy); // Add this command to the rawArgsHistory
    printf("rawArgsHistory[%d]: %s\n", index%COMMAND_HISTORY_LEN, rawArgsHistory[index%COMMAND_HISTORY_LEN]);

}

int main(void)
{
    char *args[MAX_LINE/2 + 1]; // Command-line arguments
    char rawArgs[MAX_LINE]; // The command-line arguments combined, a single string
    char rawArgsHistory[COMMAND_HISTORY_LEN][MAX_LINE]; // History of rawArgs (COMMAND_HISTORY_LEN most recent)
    int should_run = 1; // Flag to determine when to exit the program
    int ampersandSeen = 0; // Flag to determine whether an ampersand exists in the user command
    int noCommandRun = 0; // Flag to determine whether we have no command to run (1 if we DON'T run a command)
    pid_t forkResult; // Result of fork() operation
    int childStatus; // Status returned from child process
    u_int32_t currentIndex = 1; // Maintains index of current command (1-based)

    while (should_run)
    {
        printf("osh> "); // Print command prompt
        fflush(stdout); // Flush the output buffer
        resetVars(&ampersandSeen, &noCommandRun); // Reset any flags

        fgets(rawArgs, MAX_LINE, stdin); // Read the user input

        tokenizeArgs(rawArgsHistory, args, rawArgs, currentIndex, &ampersandSeen, &noCommandRun); // Tokenize the input into args

        if (args[0] == NULL || noCommandRun) // If there is no command to run...
            continue; // ...just continue

        forkResult = fork(); // Create child process

        // Child process
        if (forkResult == 0)
            execvp(args[0], args);

        // Parent process
        else if (forkResult > 0)
        {
            if (!ampersandSeen)
            {
                wait(&childStatus);
            }
        }

        else
            perror("There was an error calling the fork() function.\n");

        currentIndex++; // Increment the command index
    }

    return 0;
}