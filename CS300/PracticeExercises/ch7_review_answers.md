# Chapter 7 Review Answers

## 7.1
### If the current value of counter = 5, what are its possible values if the producer and consumer processes run concurrently?
- The possible values of counter are 4, 5, and 6 if producer and consumer processes run concurrently without proper synchronization.

## 7.2
### What is the term for describing the situation where shared data may be manipulated concurrently and the outcome of the execution depends upon the order of access?
- Race condition

## 7.3
### What is the term used to describe the segment of code where shared data is accessed and possibly manipulated?
- Critical section

## 7.4
### What are the three requirements a solution to the critical-section problem must satisfy?
1. **Mutual Exclusion:** Only up to one process may have access to a critical resource at a time
2. **Progress:** If a process is not executing in its critical section, and other process(es) are waiting tp execute on theirs, the decision for who gets to "go next" can not be postponed indefinitely
3. **Bounded Waiting:** There exists a limit on the number of times another process can be selected before a particular process to be allowed to enter its critical section. This makes no guarantees about the **amount** of time a process may spend waiting to enter its critical section, only the **number** of times another process "goes before" this process.

## 7.5
### True or False? A nonpreemptive kernel is essentially free from race conditions.
- False

## 7.6
### True or False? There are no guarantees Peterson’s solution works correctly on modern computer architectures.
- This is true! There are no guarantees that Peterson's solution works correctly on modern machines because many modern CPUs will rearrange the order of read and write operations that have no dependencies. This is an issue because this reordering of instructions may produce unexpected or inconsistent results.

## 7.7
### True or False? All solutions to the critical section problem are based on the premise of locking.
- True! 

## 7.8
### What are the two general hardware instructions that can be performed atomically?
- Compare-and-swap (CAS) and test-and-set

## 7.9
### What are the two functions used with mutex locks?
- `acquire()` and `release()`

## 7.10
### True or False? A spinlock is a type of mutex lock
True! A spinlock is a simple kind of mutex lock, wherein a process trying to get access to a critical section of code will repeatedly "ask for access" of the mutex lock, until it can finally get access. This is also referred to as "busy waiting", and in cases where the time spent busy waiting is small, this can be faster than having the process go to sleep, and then be woken when the mutex lock is available.

## 7.11
### True or False? Semaphores can provide the same functionality as mutex locks
True, but semaphores are more sophisticated. A binary semaphore can be used in place of a mutex lock to provide mutual exclusion.


## 7.14
### What are the names of the two processes associated with the bounded buffer problem?
- The **producer** and **consumer** processes

## 7.15
### How many writers may concurrently share the database with the readers-writers problem?
- In the readers-writers problem, only one writer can access the database at a given time. On the other hand, an unlimited number of readers can access the database at a given time.

## 7.16
### What is the problem if all of the philosophers simultaneously pick up their left fork?
- First of all, this problem description is silly and unintuitive when we describe it as philosophers waiting for available forks with which to eat their spaghetti. We will describe the problom as philisophers waiting for available chopsticks with which to eat their noodles. Each philosopher needs both chopsticks in order to eat their meal. If they all pick up their left chopstick simultaneously, then all of the philosophers will then attempt to pick up their right chopstick, and will block indefinitely while doing so. Here's the catch: all of the chopsticks are now already taken, so the philosophers will be waiting for their right chopstick to become available, but every philosopher is blocking, so the chopstick will never become available. This is called a "circular wait" and results in this condition that we call **deadlock**.

