# Chapter 1 Review Answers

## Section 1.1

### 1.1: What are the four components of a computer system?
1. Hardware
2. Operating System
3. Application Programs
4. Users

### 1.2: Provide at least three resources the operating system allocates.
1. CPU time for computation
2. Memory space
3. Storage space
4. I/O Devices

### 1.3: What is the common name used to refer to the operating system program?
The **kernel**.

### 1.4: What do mobile operating systems often include in addition to the core kernel?
Middleware - a set of frameworks that provide additional services to application developers

## Section 1.2

### 1.5: What is an interrupt?
An interrupt is a hardware-generated change-of-flow sent to the CPU. When the CPU receives an interrupt, it will stop whatever it was doing, find the interrupt service routine for that interrupt via an interrupt vector table, and execute that interrupt service routine, before returning to the previous location of the program counter, and continuing execution of whatever it was doing before it got interrupted by the interrupt.

### 1.6: What special operation triggers a software interrupt?
A system call generates a software interrupt, which is also known as a trap.

### 1.7 What is one advantage of using a solid state disk over a magnetic disk?
The read and write speeds onto an SSD are significantly faster than that of a magnetic disk drive. Also, there are no moving parts in an SSD, unlike a HDD, so there is a lower opportunity for hardware failure.

### 1.8 What is the difference between volatile and nonvolatile storage?
Volatile storage means that bits are only persisted with a constant power source. When you turn the power off, you lose all of the information in volatile storage. Nonvolatile storage retains its information even after the power is turned off to the device.

## Section 1.3

### 1.9: What is another term for multiprocessor system?
Multiprocessor systems are also known as parallel systems.

### 1.10: Provide at least two advantages of multiprocessor systems.
1. Increased throughput - Multiprocessor systems can run multiple processes concurrently (at once), without needing to rapidly switch between both processes.
2. Economy of scale - Buying multiple processors for a single system is cheaper than buying two discrete systems

### 1.11: True or False? The most common multiple-processor system uses asymmetric multiprocessing.
False. The most common multi-processor system uses symmetric multiprocessing where all processors are capable of performing all tasks because asymmetric multiprocessing provides only redundancy, but no increase in speed or throughput. 

### 1.12 What is the name of a multiprocessor system that uses multiple computing cores?
**Multicore**

### 1.13: How does a clustered system differ from a multicore system?
In a clustered system, separate processors on separate machines communicate over a local-area network (LAN). In a multicore system, all of the processing cores exist on the same CPU die, and use the samed shared system bus to access memory and communicate.

## Section 1.4

### 1.14: How does multiprogramming increase CPU utilization?
Multiprogramming organizes jobs in such a way that the CPU always has a job/task to execute

### 1.15: What is the term for a program that has been loaded and is executing?
A **process**

### 1.16: What part of the operating system makes the decision with regards to which job will run?
The **CPU scheduler**

## Section 1.5

### 1.17: What are the two separate modes of operation?
**user mode** and **kernel mode**

### 1.18: What is the mode of the system at boot time?
**kernel mode**

### 1.19: What is the mode of the system when the operating system gains control?
**kernel mode**

### 1.20: What is the mode of the system when a user program is running?
**user mode**

### 1.21: Name at least two activities the operating system is responsible for in connection with memory management.
1. Keeping track of which parts of memory are currently being used and who's using them
2. Allocating memory when a user program needs to store information
3. Freeing memory when a user program no longer needs to store some information
3. Deciding which processes (and data) to move in and out of memory

### 1.22: True or False? Managing files is one of the most visible aspects of an operating system.
**True**

### 1.23: Name at least two activities the operating system is responsible for in connection with file management.
1. Keeping track of directory hierarchy so that the user can find their folders and files
2. Creating/deleting files/directories
3. Managing permissions of different users/groups to files and folders

### 1.24: Name at least two activities the operating system is responsible for in connection with disk management. 
1. Free-space management
2. Storage allocation
3. Disk scheduling
4. Partitioning

### 1.25: Of the following five forms of storage, rank them from fastest to slowest in terms of access time: (1) main memory, (2) magnetic disk, (3) registers, (4) solid state disk, (5) cache.
1. Registers
2. Cache
3. Main memory
4. Solid state drive
5. Magnetic disk

## Section 1.6

### 1.26: What's the difference between protection and security?
Protection is any mechanism for controlling the access of processes or users to resources defined by a computer system. Security, on the other hand, is the defense of a system from externel *and* internal attacks.

## Section 1.7

### 1.27: A

## Section 1.8

### 1.28: A

## Section 1.9

### 1.29: List at least four common kernel data structures
1. Stack
2. Queue
3. Balanced binary tree 
4. Array
5. List
6. Hash functions and maps
7. Bitmap

### 1.30: True or False? A bitmap of length N can be used to represent the status of 2^N items.
**False**. A bitmap of length N is a string of N binary digits that can be used to represent the status of N items.

## Section 1.10

### 1.31: List at least five different types of computing environments.
1. Time-shared (like the old mainframes)
2. Web-based/cloud-based
3. Client-server
4. Mobile
5. Distributed

## Section 1.11

### 1.32: Provide an example of an open source operating system.
Ubuntu 20.04 is an example of an open-source OS. It runs on the Linux kernel version 5.4.