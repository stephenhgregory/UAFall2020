#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LINE_CHARS 1000
#define MAX_PROCESSES 1000

/**
 * Struct representing a process
 */
typedef struct process
{
    pid_t pid;
    int priority;
    int arrival_time;
    int remaining_processor_time;
} process;

/**
 * Struct represting a process node in a queue
 */
typedef struct processNode
{
    process p;
    struct processNode* next;
} processNode;

/**
 * Struct representing the system queue, which is
 * simply implemented as a FIFO queue
 */
typedef struct systemQueue
{
    int count;
    processNode* front;
    processNode* rear;
} systemQueue;

/**
 * Struct representing a jobQueue
 * If processes run from this queue do not finish in time, 
 * they get pushed down to a lower-priority queue
 */
typedef struct jobQueue
{
    int count;
    processNode* front;
    processNode* rear;
} jobQueue;

char* int_to_string_on_heap(int x);
void int_to_string_on_stack(int x, char* x_to_string);
void parse_process_line(char* processLine, process* my_process);
int insertSorted(process* processes, int n, process newProcess, int capacity);
int populate_process_list(process* processList, FILE* fp);

/**
 * Initializes a jobQueue to NULL pointers and 0
 */
void initialize(jobQueue* q)
{
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
}

/**
 * Returns 1 if a System Queue is empty, else,
 * if it isn't empty, returns 0
 */
int isEmpty(jobQueue* q)
{
    return (q->rear == NULL);
}

/**
 * Adds a new processNode to the System Queue
 */
void enqueue(jobQueue* q, process* p)
{
    processNode* tmp;
    tmp = (processNode*)malloc(sizeof(processNode));
    tmp->p = *p;
    tmp->next = NULL;
    if (!isEmpty(q))
    {
        q->rear->next = tmp;
        q->rear = tmp;
    }
    else
    {
        q->front = q->rear = tmp;
    }
    q->count++;
}

/**
 * Deletes the processNode from the front of the SystemQueue,
 * and returns a copy of the process in that node
 */
process dequeue(jobQueue* q)
{
    processNode* tmp;
    process returnProcess = q->front->p;
    tmp = q->front;
    q->front = q->front->next;
    q->count--;
    free(tmp);
    return returnProcess;
}

/**
 * Terminates a given process p.
 * 
 * If the termination is successful, function returns 0.
 * Else, if the termination is unsuccessful, function returns -1.
 * 
 * TODO: You might still need to free the memory associated with this process,
 * if the process is stored on the heap
 */
int terminateProcess(process* p)
{
    return kill(p->pid, SIGINT);
}

/**
 * Suspends a given (user) process p, and lowers its priority, if it
 * isn't already at the lowest priority of 3.
 * 
 * If the suspension of the process is successful, function returns 0.
 * Else, if the termination is unsuccesful, function returns -1.
 */
int suspendProcess(process* p)
{
    int suspendResult = kill(p->pid, SIGTSTP);
    if (p->priority > 3) 
        p->priority--;
    return suspendResult;
}

/**
 * Starts a given process p
 * 
 * If starting the process is successful, function returns 0.
 * Else, if starting the process is unsuccessful, function returns -1.
 */
int startProcess(process* p)
{
    // Fork the current process to create a new process
    pid_t pid = fork();

    // Child process
    if (pid == 0)
    {
        // Get the command line arguments
        int stringLength = snprintf(NULL, 0, "%d", p->remaining_processor_time);
        char processorTime[stringLength+1]; 
        int_to_string_on_stack(p->remaining_processor_time, processorTime);
        char* args[1];
        args[0] = processorTime;

        // Replace the current executing program with "./process" to (re)run that process
        execvp("./process", args);
    }
    // Parent process
    else if (pid > 0)
    {
        // Capture the pid of the child process into the process pointer
        p->pid = pid;
        return 0;
    }    
    // Otherwise, fork() must have failed
    return -1;
}

/**
 * Restarts a given (user) process p
 * 
 * If restarting the process is successful, function returns 0.
 * Else, if restarting the process is unsuccessful, function returns -1.
 */
int restartProcess(process* p)
{
    // Fork the current process to create a new process
    pid_t pid = fork();

    // Child process
    if (pid == 0)
    {
        // Get the command line arguments
        int stringLength = snprintf(NULL, 0, "%d", p->remaining_processor_time);
        char processorTime[stringLength+1]; 
        int_to_string_on_stack(p->remaining_processor_time, processorTime);
        char* args[1];
        args[0] = processorTime;

        // Replace the current executing program with "./process" to (re)run that process
        execvp("./process", args);
    }
    // Parent process
    else if (pid > 0)
    {
        // Do nothing, and just return 0
        return 0;
    }    
    // Otherwise, fork() must have failed
    return -1;
}

/**
 * Converts a given integer to a string, and returns that string
 * 
 * NOTE: Allocates memory for the new char*, which must be freed later.
 */
char* int_to_string_on_heap(int x)
{
    // Get the length necessary for the string
    int length = snprintf(NULL, 0, "%d", x);

    // Allocate space for (length+1) chars
    char* x_to_string = (char*)malloc(sizeof(char)*(length+1));

    // Convert the integer into a string
    sprintf(x_to_string, "%d", x);

    return x_to_string;
}

/**
 * Converts a given integer to a string, and returns that string
 */
void int_to_string_on_stack(int x, char* x_to_string)
{
    // Convert the integer into a string
    sprintf(x_to_string, "%d", x);
}


/**
 * Parses a string containing <arrival time>, <priority>, <processor time>,
 * and populates a process "my_process", which is passed-in to the function
 * as a process*, with those values. 
 * 
 */
void parse_process_line(char* processLine, process* my_process)
{
    // TODO: Parse the input line into <arrival time>, <priority>, <processor time>,
    // and create a new process object for that process.

    // Get the arrival time, priority, and processor time (as strings)
    char* arrivalTimeString = strtok(processLine, ", \n");
    char* priorityString = strtok(NULL, ", \n");
    char* processorTimeString = strtok(NULL, ", \n");

    // Convert the arrival time priority, and processor time to ints
    int arrivalTime = atoi(arrivalTimeString);
    int priority = atoi(priorityString);
    int processorTime = atoi(processorTimeString);

    // Populate my_process
    my_process->arrival_time = arrivalTime;
    my_process->priority = priority;
    my_process->remaining_processor_time = processorTime;
    my_process->pid = -1;
}

/**
 * Inserts a process in arr[] of given capacity. n is current
 * size of arr[]. This function returns n+1 if insertion
 * is successful, else n.
 */
int insertSorted(process* processes, int n, process newProcess, int capacity)
{
    // Cannot insert more elements if n is already
    // more than or equal to capcity
    if (n >= capacity)
        return n;
    int i;

    for (i = n - 1; (i >= 0 && processes[i].arrival_time > newProcess.arrival_time); i--)
        processes[i + 1] = processes[i];
 
    processes[i + 1] = newProcess;
 
    return (n + 1);
}

/**
 * Reads a list of processes from a file through the passed-in FILE pointer,
 * and adds all of them in sorted, non-decreasing order of arrival time 
 * into the passed-in "processList".
 * 
 * Returns the total length of the processList.
 */
int populate_process_list(process* processList, FILE* fp)
{
    int counter = 0;
    char processLine[MAX_LINE_CHARS];

    // Read a line from the file into "processLine" until EOF
    while (fgets(processLine, MAX_LINE_CHARS, fp))
    {
        // Create a new process for this line, and populate its 
        // arrival time, priority, and processor time.
        process newProcess;
        parse_process_line(processLine, &newProcess);

        // Insert the process into the (sorted by arrival time)
        // process list, and increment the size counter.
        insertSorted(processList, counter, newProcess, MAX_PROCESSES);
        counter++;
    }

    return counter;
}

/**
 * Prints all of the processes in the processList
 */
void print_process_list(process* processList, int processListSize)
{
    int i;
    for (i = 0; i < processListSize; i++)
    {
        printf("processList[%d].arrival_time = %d\n", i, processList[i].arrival_time);
    }
}


int main(int argc, char** argv)
{
    FILE* fp;
    process processList[MAX_PROCESSES];
    int numProcesses;
    int latestArrival = 0;

    jobQueue systemQueue;
    jobQueue userQueueHighPriority;
    jobQueue userQueueMidPriority;
    jobQueue userQueueLowPriority;

    // Get the filename from the command line
    char* filename = argv[1];
    // Open the file containing processes
    fp = fopen(filename, "r");

    // Exit if the file could not be opened
    if (fp == NULL)
    {
        printf("Could not open file \"%s\". Exiting now...\n", filename);
        return 1;
    }

    // Add all of the processes to the list of processes in sorted order
    // of non-decreasing arrival time.
    numProcesses = populate_process_list(processList, fp);
    print_process_list(processList, numProcesses);

    


    printf("Goodbye, world!\n");
    return 0;
}