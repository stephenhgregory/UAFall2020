# Chapter 6 Practice Exercise Answers

## 6.1
### In Section 6.4, we mentioned that disabling interrupts frequently can affect the system’s clock. Explain why this can occur and how such effects can be minimized. 
- The system clock is updated at every clock interrupt. If interrupts were disabled - particularly for a long time - it is possible that the system clock could easily lose the correct time.
Also, the system clock is used for scheduling purposes. For example, the time quantum for a process is expressed as a number of clock ticks. Here, at every clock interrupt, the scheduler determines if the time quantum for the currently running process has expired. If these clock interrupts were disabled, the scheduler could not accurately assign time quanta. These negative effects can be mitigated by disabling clock interrupts only for a very short time. 

## 6.2 
### What is the meaning of the term busy waiting? What other kinds of waiting are there in an operating system? Can busy waiting be avoided altogether? Explain your answer. 
- Busy waiting simply means that a process is waiting for a condition to be satisfied in a tight loop without relinquishing the processor. Alternatively, the waiting process could wait by relinquishing the processor, and block on a condition and wait to be awakened at some time in the future. Busy waiting can be avoided, but avoiding busy waiting incurs the overhead of putting a process to sleep, and then needing to wake the process back up when the appropriate program state is reached.

## 6.3
### Explain why spinlocks are not appropriate for single-processor systems yet are often used in multiprocessor systems. 
- Spinlocking would not work for a single-process system because the condition that would break the process out of that spinlock can only be obtained by executing a different process. If the process that is spinlocking is not relinquishing the processor, other processes do not get the chance to set the program condition required for the first process to make progress. In a multiprocessor system, other processes run on other processors and therefore modify the program state in order to release the first process from the spinlock.

## 6.4
### Show that, if the wait() and signal() semaphore operations are not executed atomically, then mutual exclusion may be violated. 
- A ```wait()``` operation atomically decrements the value associated with a semaphore. If two ```wait()``` operations are executed on a semaphore when its value is initially 1, and the two operations are **not** performed atomically, then it's possible that both operations might proceed to decrement the semaphore value, thereby violating mutual exclusion.

## 6.5
### Illustrate how a binary semaphore can be used to implement mutual exclusion among n processes.
- Quite simply, a binary semaphore ```mutex``` could be initialized with a value of 1, and shared among all *n* processes. Then, any time a process wanted to access some data ```x```, the process would first need to call ```wait()``` on semaphore ```mutex```, and after that process is done accessing ```x```, it would call ```signal()``` on the semaphore ```mutex``` again, like so:
```
do 
{
    wait(mutex);

        /* critical section */

    signal(mutex);

        /* remainder section
} while (true);
```

## 6.6
### Race conditions are possible in many computer systems. Consider a banking system that maintains an account balance with two functions: deposit(amount) and withdraw(amount). These two functions are  passed the amount that is to be deposited or withdrawn from the bank account balance. Assume that a husband and wife share a bank account. Concurrently, the husband calls the withdraw() function, and the wife calls deposit(). Describe how a race condition is possible and what might be done to prevent the race condition from occurring. 
- One instance of a race condition could occur if access to the same shared variable ```balance``` was allowed without mutual exclusion. For example, imagine that the value of ```balance``` is 100. The wife calls ```deposit(50)```, while the husband calls ```withdraw(100)```. First, the husband's program calls ```withdraw(100)``` and accesses the current value of 100. However, before the new current balance is stored, the wife's program calls ```deposit(50)``` and changes the value of ```balance``` to 100 + 50 = 150. Then, the husband's program changes the value to 0, followed by the wife's program changing the value to 150. Finally, we see that even though the expected value of ```balance``` is 50, we end up with a ```balance``` of 150. The obvious solution here is to create a "lock" on the shared variable ```balance```. This way, any process that wants to access ```balance``` must first "lock" the variable before accessing/changing it, after which that process may "unlock" the variable.

## 6.7
### The pseudocode of Figure 6.15 illustrates the basic push() and pop() operations of an array-based stack. Assuming that this algorithm could be used in a concurrent environment, answer the following questions: 
### 1. What data have a race condition?
### 2. How could the race condition be fixed?
1. Here, ```isempty()``` does **not** have a race condition, as it does not modify the top variable. However, ```push()``` and ```pop()``` both modify the ```top``` variable, and are susceptible to a race condition. For example, assume process ```A``` calls ```push()``` and process ```B``` calls ```pop()``` at the same time. Then, process ```A``` executes ```stack[top] = item``` just as ```A``` enters its if statement. Then, suppose that process ```B``` makes a call to ```top--``` at the same time that process ```A``` makes a call to ```top++```. Because this operation is not atomic, it is possible that ```top``` is not decremented as it is expected to. In this case, process `B` will attempt to access `stack[top]`, which will result in a reference to undefined memory, and a resultant segmentation fault.
2. Placing a binary semaphore or mutex lock into the functions could fix these race conditions. In the case of a binary semaphore, we would first need to initialize a semaphore named `sem` and initialize the value to 1. Then, in `push()`, we would place a call to `wait(sem)` before the call to `top++`, immediately followed by a call to `signal(sem)`. Likewise, in `pop()`, we would place a call to `wait(sem)` before the call to `top--`, immediately followed by a call to `signal(sem)`. This would ensure mutual exclusion and avoid a race condition.

## 6.8
### Race conditions are possible in many computer systems. Consider an online auction system where the current highest bid for each item must be maintained. A person who wishes to bid on an item calls the bid(amount) function, which compares the amount being bid to the current highest bid. If the amount exceeds the current highest bid, the highest bid is set to the new amount. This is illustrated below: 
```
void bid(double amount) 
{
    if (amount > highestBid)
        highestBid = amount;
}
```
### Describe how a race condition is possible in this situation and what might be done to prevent the race condition from occurring.
- Suppose two processes (people) call the function `bid(amount)` at the same time. Before these calls, `highestBid == 100`, `process1_amount == 150`, and `process2_amount == 200`. Then, both `process1` and `process2` execute the condition inside of the if-statement at the same time (meaning that they are both higher than the current highestBid). Then, assume that `process2` executes the assignment `highestBid = amount` first, setting `highestBid == 200`. Immediately following, `process1` executes the assignment `highestBid = amount`, setting `highestBid == 150`. Therefore, we now have that the highest bidding is **150**, even though `process2` put forward a bid of **200**.
- We can see that the shared variable which requires mutual exclusion is `highestBid`. If we initialize a binary semaphore variable to `1`, then modify the code as follows, we can see that we do not have the previously described race condition.
```
Semaphore sem = 1;

...
...
...

void bid(double amount) 
{
    wait(sem);

    /* Critical Section */
    if (amount > highestBid)
        highestBid = amount;

    signal(sem);
}
```

## 6.9
### The following program example can be used to sum the array values of size N elements in parallel on a system containing N computing cores (there is a separate processor for each array element): 

