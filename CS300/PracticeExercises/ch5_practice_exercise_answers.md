# Chapter 5 Practice Exercise Answers

## 5.1 
### A CPU-scheduling algorithm determines an order for the execution of its scheduled processes. Given n processes to be scheduled on one processor, how many different schedules are possible? Give a formula in terms of n. 
- This question can be generalized to a more common problem: how many different orderings of n items in a set are possible? This is a permutation! A permutation ```P(n, r)``` tells us how many orderings of ```n``` items in a set are possible, when taken ```r``` at a time. Here, the number of items that we "take at a time" is same as the length of the set itself. In other words, we're trying to order the entire set of processes. the formula for permutations is as follows: 
  - ```P(n, r) = (n!)/(n - r)!```
- Because ```n == r``` in our case, the solution to this problem is simply the factorial of n, ```n!```.

## 5.2
### Explain the difference between preemptive and non-preemptive scheduling
- Preemptive scheduling means that the CPU is going to *preempt* processes when it decides to. This means that if the CPU has given a process a certain time to execute, and it wants another process to execute, the CPU will *preempt* the running process, stopping that process in its tracks, and handing control to the other process that it wants to run. In other words, preemptive scheduling means that the processor is allowed to stop processes whenever it wants, it doesn't have to wait on the processes to give up control. Conversely, non-preemptive scheduling means that the CPU will **not** *preempt* running processes. This means that the CPU will not stop a process from running to hand control to another process. The CPU will be patient, and will wait until processes finish execution and willingly give up control themselves back to the CPU. 

## 5.3
### Suppose that the following processes arrive for execution at the times indicated. Each process will run for the amount of time listed. In answering the questions, use nonpreemptive scheduling, and base all decisions on the information you have at the time the decision must be made. 
| **Process** | **Arrival Time** | **Burst Time** |
|-------------|------------------|----------------|
| P1          | 0.0              | 8              |
| P2          | 0.4              | 4              |
| P3          | 1.0              | 1              |
### 1. What is the average turnaround time for those processes with the FCFS scheduling algorithm?
1. Recall that turnaround time is the difference between finish time and arrival time.
2. P1 => 8 - 0
3. P2 => 12 - 0.4
4. P3 => 13 - 1
5. P1 + P2 + P3 / 3 = (8 + 11.6 + 12) / 3 = 31.6 / 3 ≈ **10.533**
### 2. What is the average turnaround time for those processes with the SJF scheduling algorithm?
1. P1 => 8 - 0
2. P3 => 9 - 1
3. P2 => 13 - 0.4
4. P1 + P2 + P3 / 3 = (8 + 8 + 12.6) / 3 = 28.6 / 3 ≈ **9.533**
### 3. The SJF algorithm is supposed to improve performance, but notice that we chose to run process P1 at time 0 because we did not know that two shorter processes would arrive soon. Compute what the average turnaround time will be if the CPU is left idle for the first 1 unit and then SJF scheduling is used. Remember that processes P1 and P2 are waiting during this idle time, so their waiting time may increase. This algorithm could be known as future-knowledge scheduling. 
1. Notice we are actually beginning to schedule our processes at time 1
2. P3 => 2 - 1
3. P2 => 6 - 0.4
4. P1 => 14 - 0
5. P1 + P2 + P3 / 3 = (1 + 5.6 + 14) / 3 = 20.6 / 3 ≈ **6.5867**

## 5.4
### Consider the following set of processes, with the length of the CPU burst time given in milliseconds
| **Process** | **Burst Time** | **Priority** |
|-------------|----------------|--------------|
| P1          | 2              | 2            |
| P2          | 1              | 1            |
| P3          | 8              | 4            |
| P4          | 4              | 2            |
| P5          | 5              | 3            |
### 1. Draw four Gantt charts that illustrate the execution of these processes using the following scheduling algorithms: FCFS, SJF, nonpreemptive priority (a larger priority number implies a higher  priority), and RR (quantum = 2).
- *See Notability*
### 2. What is the turnaround time of each process for each of the scheduling algorithms in part a?
- *See Notability*
### 3. What is the waiting time of each process for each of these scheduling algorithms?
- *See Notability*
### 4. Which of the algorithms results in the minimum average waiting time (over all processes)?
- SJF at 4.6 units of time

## 5.5
### *See Notability*

## 5.6
### What advantage is there in having different time-quantum sizes at different levels of a multilevel queueing system?
- Processes that may need a larger amount of interactivity and more frequent I/O bursts could benefit from having a shorter time quantum than much more CPU-bound processes, or processes that simply need raw computational time.

## 5.7
### Many CPU-scheduling algorithms are parameterized. For example, the RR algorithm requires a parameter to indicate the time slice. Multilevel feedback queues require parameters to define the number of queues, the scheduling algorithms for each queue, the criteria used to move processes between queues, and so on. These algorithms are thus really sets of algorithms (for example, the set of RR algorithms for all time slices, and so on). One set of algorithms may include another (for example, the FCFS algorithm is the RR algorithm with an infinite time quantum). What (if any) relation holds between the following pairs of algorithm sets? 
### 1. Priority and SJF
- SJF could be defined as an algorithm wherein the CPU burst time is the representation of priority, where the lowest CPU burst time is the highest priority.
### 2. Multilevel feedback queues and FCFS
- In fact, the lowest-level queue in a MLFQ is a FCFS queue. This happens so that processes don't simply become starved.
### 3. Priority and FCFS
- In a FCFS, the time of arrival is essentially the priority, with the most recent arrivals having the highest priority. In this way, FCFS gives the highest priority to those jobs that have been in existence for the longest time.
### 4. RR and SJF
- Round Robin and Shortest Job First don't *really* have that much in common with eachother.

## 5.8
### Suppose that a CPU scheduling algorithm favors those processes that have used the least processor time in the recent past. Why will this algorithm favor I/O-bound programs and yet not permanently starve CPU-bound programs? 
- The CPU will favor the I/O-bound processes because of their relatively short CPU bursts; however, the CPU-bound processes will not starve because the I/O-bound processes will frequently relinquish the CPU to do their I/O.

- NOTE: If this algorithm was implemented as a multilevel feedback queue, then a process which requires more processor time would certainly be moved to a lower priority queue, but in that queue, it would certainly be given a longer time quantum. In the absolutely worst case, that CPU-bound process might move to the bottom queue (priority-wise), where it would sit in a FCFS queue with no time quantum at all, and would be always be the first process to run in its queue until it either completes execution or is preempted by a process in a higher-priority queue.

## 5.9
### Distinguish between PCS and SCS scheduling 
- PCS is implemented on many-to-many and one-to-many while SCS is implemented on one-to-one models
- In PCS, scheduling is local so the threads competing belong to the same process
- PCS is a priority based scheduler where priorities are set by a programmer
- In PCS, threads are scheduled on available LWPs
- SCS is used by the kernel to decide which kernel-level thread to schedule onto a CPU
- In SCS, all of the threads on the system compete for the CPU
- My answer: PCS stands for process-contention scope, while SCS stands for system-contention scope. PCS refers to the scheduling that occurs when threads belonging to the same process compete for CPU time. SCS refers to the scheduling that occurs between all of the threads in the system. Systems using the many-to-many model will use PCS to handle competition among user threads attached to a single LWP or kernel thread, and SCS to handle competition among separate LWPs. Systems using the one-to-one model, on the other hand, will typically only use SCS to handle the competition among separate threads, all of which are mapped directly to kernel threads.

## 5.10
### The traditional UNIX scheduler enforces an inverse relationship between priority numbers and priorities: the higher the number, the lower the priority. The scheduler recalculates process priorities once per second using the following function: 
    Priority = (recent CPU usage / 2) + base
### where base = 60 and recent CPU usage refers to a value indicating how often a process has used the CPU since priorities were last recalculated. Assume that recent CPU usage for process P1 is 40, for process P2 is 18, and for process P3 is 10. What will be the new priorities for these three processes when priorities are recalculated? Based on this information, does the traditional UNIX scheduler raise or lower the relative priority of a CPU-bound process? 
- P1 will have a priority of 80, P2 will have a priority of 69, and P3 will have a priority of 65. Based upon this information, we can see that there is a direct relationship between CPU usage and priority number, and an inverse relationship between CPU usage and priority, by transience. P1 in this case has the highest level of recent CPU usage, and therefore is given the lowest priority.