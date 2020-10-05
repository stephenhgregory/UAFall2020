#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LINE_CHARS 1000
#define MAX_PROCESSES 1000
#define TIME_QUANTUM 1

// Comment this line of code out when not running from VS Code's debugger
#define VSCODE_DEBUG 1

// Comment this line of code out when not using extended debugging
// (extra print statements)
#define EXTENDED_DEBUG 1


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

void initialize(jobQueue* q);
int isEmpty(jobQueue* q);
void enqueue(jobQueue* q, process p);
process dequeue(jobQueue* q);
int terminateProcess(process* p);
int suspendProcess(process* p);
int startProcess(process* p);
int restartProcess(process* p);
void int_to_string_on_stack(int x, char* x_to_string);
void parse_process_line(char* processLine, process* my_process);
int insertSorted(process* processes, int n, process newProcess, int capacity);
void populate_process_list(process* processList, FILE* fp, int* numProcesses, int* latestArrival);
void updateQueuesWithArrivals(int timestep, int i, const int processListSize, process* processList, jobQueue* systemQueue, jobQueue* userQueueHighPriority, jobQueue* userQueueMidPriority, jobQueue* userQueueLowPriority);
void print_process_list(process* processList, const int processListSize, const int latestArrival);
void runProcesses(process* processList, const int processListSize, const int latestArrival);

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
    int result = (q->rear == NULL);
    return result;
}

/**
 * Adds a new processNode to a jobQueue
 */
void enqueue(jobQueue* q, process p)
{
    // Create a new node for the queue
    processNode* tmp;
    tmp = (processNode*)malloc(sizeof(processNode));
    tmp->p = p;
    tmp->next = NULL;

    // If the queue is not empty, then add the new node at
    // the rear of the queue and change rear.
    if (!isEmpty(q))
    {
        q->rear->next = tmp;
        q->rear = tmp;
    }
    // Else, if the queue is empty, then the new node 
    // is both the front and the rear of the queue
    else
    {
        q->front = q->rear = tmp;
    }

    // Finally, update the total count of the queue
    q->count++;
}

/**
 * Deletes the processNode from the front of a jobQueue,
 * and returns a copy of the process in that node
 * 
 * NOTE: This function assumes that the queue is NOT empty
 */
process dequeue(jobQueue* q)
{
    // Store the previous front and move front one ahead
    processNode* tmp;
    process returnProcess = q->front->p;
    tmp = q->front;
    q->front = q->front->next;
    q->count--;

    // If front becomes NULL, then change rear to NULL as well
    if (q->front == NULL)
        q->rear = NULL;

    // Free the memory of the now chopped-off processNode, and
    // return the process from that node.
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
 * NOTE: This function modifies the process p by setting its pid field
 * equal to the pid of the process.
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
        char* args[2];
        args[0] = processorTime;
        args[1] = NULL;

        // Replace the current executing program with "./process" to run that process
        if (execvp("./process", args))
        {
            printf("Error calling execvp().\n");
            exit(1);
        }
        exit(0);
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
    // Send a signal to the process to start again
    return kill(p->pid, SIGCONT);
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
void populate_process_list(process* processList, FILE* fp, int* numProcesses, int* latestArrival)
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

        // Keep track of the latest arrival time in the entire list of processes
        if (newProcess.arrival_time > *latestArrival)
            *latestArrival = newProcess.arrival_time;

        counter++;
    }

    *numProcesses = counter;
}

/**
 * Prints all of the processes in the processList
 */
void print_process_list(process* processList, const int processListSize, const int latestArrival)
{
    int i;
    for (i = 0; i < processListSize; i++)
    {
        printf("processList[%d].arrival_time = %d\n", i, processList[i].arrival_time);
    }
    printf("Latest arrival time = %d\n", latestArrival);
}


void updateQueuesWithArrivals(int timestep, int i, const int processListSize, process* processList, jobQueue* systemQueue, jobQueue* userQueueHighPriority, jobQueue* userQueueMidPriority, jobQueue* userQueueLowPriority)
{

    // If there is a process(s) at OR BEFORE this arrival time, place it (them) 
    // in the proper queues
    if (timestep >= processList[i].arrival_time)
    {
        // Create temporary queue to store processes with the same arrival time
        jobQueue arrivalQueue;
        initialize(&arrivalQueue);
        int arrivalQueueSize = 0;
        int j = i+1;

        enqueue(&arrivalQueue, processList[i]);
        arrivalQueueSize++;

        // Keep adding processes with the same arrival time to the temporary queue
        while(j < processListSize && processList[j].arrival_time == processList[i].arrival_time)
        {
            enqueue(&arrivalQueue, processList[j]);
            j++;
        }

        // Add each process at this arrival time to its proper queue 
        while (!isEmpty(&arrivalQueue))
        {
            process my_process = dequeue(&arrivalQueue);

            switch (my_process.priority)
            {
                // System queue
                case 0:
                    enqueue(systemQueue, my_process);
                    break;

                // Highest-priority user Queue
                case 1:
                    enqueue(userQueueHighPriority, my_process);
                    break;

                // Medium-priority user Queue
                case 2:
                    enqueue(userQueueMidPriority, my_process);
                    break;

                // Low-priority user Queue
                case 3:
                    enqueue(userQueueLowPriority, my_process);
                    break;
            }
        }
    }
}


/**
 * Runs all of the processes in the process list
 */
void runProcesses(process* processList, const int processListSize, const int latestArrival)
{
    // Create and initialize all 4 queues
    jobQueue systemQueue;
    jobQueue userQueueHighPriority;
    jobQueue userQueueMidPriority;
    jobQueue userQueueLowPriority;
    initialize(&systemQueue);
    initialize(&userQueueHighPriority);
    initialize(&userQueueMidPriority);
    initialize(&userQueueLowPriority);

    int i = 0;
    int timestep = 0;
    int status;
    int processRunResult = 0;

    // Iterate over until we've added every single process in the processList
    while (i < processListSize)
    {
        // Add any processes that have arrived at this timestep to the appropriate queues
        updateQueuesWithArrivals(timestep, i, processListSize, processList, &systemQueue, &userQueueHighPriority, &userQueueMidPriority, &userQueueLowPriority);

        // If the system Queue has anything in it...
        if (!isEmpty(&systemQueue))
        {
            // TODO: 
            // 1. Run what is in the system Queue.
            // 2. Update the timestep by the amount of CPU time that was just used.
            // 3. Remove that process from the system Queue.

            // Get the process to run from the front of the system queue
            process process_to_run = dequeue(&systemQueue);

            // If the process did not already exist, start the process.
            if (process_to_run.pid == -1)
            {
                printf("Starting process now (process.pid = %d, process.arrival_time = %d, process.priority = %d, process_to_run.remaining_processor_time = %d)...\n", process_to_run.pid, process_to_run.arrival_time, process_to_run.priority, process_to_run.remaining_processor_time);
                processRunResult = startProcess(&process_to_run);
            }
            // Else, if it did already exist, restart it.
            else
            {
                printf("Restarting process now...\n");
                processRunResult = restartProcess(&process_to_run);
            }
                
            // Wait for the process to finish its execution
            while (waitpid(process_to_run.pid, &status, WUNTRACED));

            // Account for the processing time used by the process
            timestep += process_to_run.remaining_processor_time;

            // Return with an error message if there was a problem
            if (processRunResult)
            {
                perror("There was an error running a process.");
                return;
            }

            continue;
        }
        // Else if the highest-priority job queue has anything in it...
        else if (!isEmpty(&userQueueHighPriority))
        {
            // TODO: 
            // 1. Run what is in userQueueHighPriority.
            // 2. Update the timestep by the amount of CPU time that was just used.
            // 3. 
            //      If the process finished in time, simply remove it from the queue.
            //      Else if the process was preempted, move it to the userQueueMidPriority.

            printf("Inside of high priority user Queue...\n");

            // Get the process to run from the front of the highest-priority user Queue
            process process_to_run = dequeue(&userQueueHighPriority);

            // If the process did not already exist, start the process.
            if (process_to_run.pid == -1)
            {
                printf("Starting process now (process.pid = %d, process.arrival_time = %d, process.priority = %d, process_to_run.remaining_processor_time = %d)...\n", process_to_run.pid, process_to_run.arrival_time, process_to_run.priority, process_to_run.remaining_processor_time);
                processRunResult = startProcess(&process_to_run);
                printf("Done starting process (process.pid = %d, process.arrival_time = %d, process.priority = %d, process_to_run.remaining_processor_time = %d)...\n", process_to_run.pid, process_to_run.arrival_time, process_to_run.priority, process_to_run.remaining_processor_time);
            }
            // Else, if it did already exist, restart it.
            else
            {
                printf("Restarting process now...\n");
                processRunResult = restartProcess(&process_to_run);
            }

            // Wait for one time quantum
            sleep(TIME_QUANTUM);

            // Decrement the remaining processor time for the process by one time quantum
            process_to_run.remaining_processor_time -= TIME_QUANTUM;

            printf("Just decremented processor time (process.pid = %d, process.arrival_time = %d, process.priority = %d, process_to_run.remaining_processor_time = %d)...\n", process_to_run.pid, process_to_run.arrival_time, process_to_run.priority, process_to_run.remaining_processor_time);

            // If the process has no remaining processor time...
            if (process_to_run.remaining_processor_time < 1)
            {
                // Terminate the process
                terminateProcess(&process_to_run);
            }
            // Else, if there is still remaining processing time...
            else 
            {
                // Suspend the process
                suspendProcess(&process_to_run);

                // Wait on the process to respond before continuing
                waitpid(process_to_run.pid, &status, WUNTRACED);

                // Demote this process to the lower priority user queue
                process_to_run.priority--;
                enqueue(&userQueueMidPriority, process_to_run);
            }

            // Add one time quantum to the current timestep
            timestep += TIME_QUANTUM;
            continue;
        }
        // Else if the medium-priority job queue has anything in it...
        else if (!isEmpty(&userQueueMidPriority))
        {
            // TODO: 
            // 1. Run what is in userQueueMidPriority.
            // 2. Update the timestep by the amount of CPU time that was just used.
            // 3. 
            //      If the process finished in time, simply remove it from the queue.
            //      Else if the process was preempted, move it to the userQueueLowPriority.

            printf("Inside of medium priority user Queue...\n");


            // Get the process to run from the front of the highest-priority user Queue
            process process_to_run = dequeue(&userQueueMidPriority);

            // If the process did not already exist, start the process.
            if (process_to_run.pid == -1)
            {
                printf("Starting process now (process.pid = %d, process.arrival_time = %d, process.priority = %d, process_to_run.remaining_processor_time = %d)...\n", process_to_run.pid, process_to_run.arrival_time, process_to_run.priority, process_to_run.remaining_processor_time);
                processRunResult = startProcess(&process_to_run);
            }
            // Else, if it did already exist, restart it.
            else
            {
                printf("Restarting process now...\n");
                processRunResult = restartProcess(&process_to_run);
            }

            // Wait for one time quantum
            sleep(TIME_QUANTUM);

            // Decrement the remaining processor time for the process by one time quantum
            process_to_run.remaining_processor_time -= TIME_QUANTUM;

            // If the process has no remaining processor time...
            if (process_to_run.remaining_processor_time < 1)
            {
                // Terminate the process
                terminateProcess(&process_to_run);
            }
            // Else, if there is still remaining processing time...
            else 
            {
                // Suspend the process
                suspendProcess(&process_to_run);

                // Wait on the process to respond before continuing
                waitpid(process_to_run.pid, &status, WUNTRACED);

                // Demote this process to the lower priority user queue
                process_to_run.priority--;
                enqueue(&userQueueLowPriority, process_to_run);
            }

            // Add one time quantum to the current timestep
            timestep += TIME_QUANTUM;
            continue;
        }
        // Else if the lowest-priority job queue has anything in it...
        else if (!isEmpty(&userQueueLowPriority))
        {
            // TODO: 
            // 1. Run what is in userQueueLowPriority.
            // 2. Update the timestep by the amount of CPU time that was just used.
            // 3. 
            //      If the process finished in time, simply remove it from the queue.
            //      Else if the process was preempted, move it back around to the 
            //          userQueueLowPriority in a circular fashion.

            printf("Inside of low priority user Queue...\n");

            
            // Get the process to run from the front of the lowest-priority user Queue
            process process_to_run = dequeue(&userQueueLowPriority);

            // If the process did not already exist, start the process.
            if (process_to_run.pid == -1)
            {
                printf("Starting process now (process.pid = %d, process.arrival_time = %d, process.priority = %d, process_to_run.remaining_processor_time = %d)...\n", process_to_run.pid, process_to_run.arrival_time, process_to_run.priority, process_to_run.remaining_processor_time);
                processRunResult = startProcess(&process_to_run);
            }
            // Else, if it did already exist, restart it.
            else
            {
                printf("Restarting process now...\n");
                processRunResult = restartProcess(&process_to_run);
            }

            // Wait for one time quantum
            sleep(TIME_QUANTUM);

            // Decrement the remaining processor time for the process by one time quantum
            process_to_run.remaining_processor_time -= TIME_QUANTUM;

            // If the process has no remaining processor time...
            if (process_to_run.remaining_processor_time < 1)
            {
                // Terminate the process
                terminateProcess(&process_to_run);
            }
            // Else, if there is still remaining processing time...
            else 
            {
                // Suspend the process
                suspendProcess(&process_to_run);

                // Wait on the process to respond before continuing
                waitpid(process_to_run.pid, &status, WUNTRACED);

                // Return this proceses to the rear of the same lowest 
                // priority user queue
                process_to_run.priority--;
                enqueue(&userQueueLowPriority, process_to_run);
            }

            // Add one time quantum to the current timestep
            timestep += TIME_QUANTUM;
            continue;
        }
        // Else, if there are no jobs to run at all...
        else
        {
            // Sleep for one time quantum and increment the timestep by one time quantum
            sleep(TIME_QUANTUM);
            timestep += TIME_QUANTUM;
            continue;
        }
    }

}


int main(int argc, char** argv)
{
    FILE* fp;
    process processList[MAX_PROCESSES];
    int numProcesses;
    int latestArrival = 0;

    // Get the filename from the command line
#ifdef VSCODE_DEBUG
    char* filename = "sampleInput.txt";
#else
    char* filename = argv[1];
#endif

    // Open the file containing processes
    fp = fopen(filename, "r");

    // Exit if the file could not be opened
    if (fp == NULL)
    {
        fprintf(stderr, "Could not open file \"%s\". Exiting now...\n", filename);
        return 1;
    }

    // Add all of the processes to the list of processes in sorted order
    // of non-decreasing arrival time.
    populate_process_list(processList, fp, &numProcesses, &latestArrival);

#ifdef EXTENDED_DEBUG
    print_process_list(processList, numProcesses, latestArrival);
#endif

    // Run the processes in the processList
    runProcesses(processList, numProcesses, latestArrival);

    printf("Goodbye, world!\n");
    fclose(fp);
    return 0;
}