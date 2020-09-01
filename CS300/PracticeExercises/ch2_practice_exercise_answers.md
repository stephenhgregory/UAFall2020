# Chapter 1 Practice Exercise Answers

## 2.1
### What is the purpose of system calls?
System calls allow user-level processes to request services of the operating system.

## 2.2
### What is the purpose of the command interpreter?
The command interpreter is the part of the operating system that executes commands that are entered interactively by a user of the system, or entered from a program. This is also called the shell in some operating systems.
### Why is it usually separate from the kernel?
The kernel is the central module of an operating system, as we know. It's the part of the operating system that is loaded first by the boot loader, and remains in main memory. Because it always stays in memory, it is important for the kernel to be as small as possible, while still providing all of the essential services required by other parts of the OS and applications. Typically, the kernel is responsible for memory management, process and task management, and disk management.