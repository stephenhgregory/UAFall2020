# Chapter 1 Practice Exercise Answers

## 2.1
### What is the purpose of system calls?
System calls allow user-level processes to request services of the operating system.

## 2.2
### What is the purpose of the command interpreter?
The command interpreter is the part of the operating system that executes commands that are entered interactively by a user of the system, or entered from a program. This is also called the shell in some operating systems.
### Why is it usually separate from the kernel?
The kernel is the central module of an operating system, as we know. It's the part of the operating system that is loaded first by the boot loader, and remains in main memory. Because it always stays in memory, it is important for the kernel to be as small as possible, while still providing all of the essential services required by other parts of the OS and applications. Typically, the kernel is responsible for memory management, process and task management, and disk management.

## 2.3 What system calls have to be executed by a command interpreter or shell in order to start a new process on a linux system?
In Unix-based systems such as systems based on the linux kernel or mach microkernel, the system call ```fork()``` is made to create a new process, which is called a child process. Then, the ```exec()``` system call is generally used to execute a file which is residing in an active process, so in our case, it's used to execute the file residing in the new process that we just created with our ```fork()``` system call.

## 2.4 What is the purpose of system programs?
System programs can be thought of as bundles of system calls. They provide basic functionality to users so that the users don't need to write their own programs to solve basic, common problems.

## 2.5 What is the main advantage of the layered approach to system design? What are the disadvantages of the layered approach?
- My answer: 
    - One of the main advantage to the layered approach to system design is isolation of concerns. If a developer is working on the 'top' layer, which might be the UI layer, and they are trying to fix some bugs in that layer, they can know with certainty that the only thing that layer is responsible for is UI. Therefore, if there is a bug with the UI, and the engineer knows that the lower layers are working normally, then they can say with confidence that the bug is isolated to the top UI layer. Also, this separation of concerns makes it easier to develop and maintain. Technically, one developer could work on the outermost UI layer, and simply use a stub for the inner layeres, while another developer could work on a lower layer. However, there are disadvantages. Firstly, this layered approach brings with it a greater overhead cost and adds complexity to the OS. This complexity could bring bugs and difficult maintenance. Furthermore, there must be some sort of message-passing protocol so that layers can communicate, and this could bring more complexity and slower performance. If the outermost layer needs to make a call to the innermost layer, they would have to send some messages or call some functions in every single intermediate layer.
- Book's Answer: 
    - As in all cases of modular design, designing an operating system in a modular way has several advantages. The system is easier to debug and modify because changes affect only limited sections of the system rather than touching all sections of the operating system. Information is kept only where it is needed and is accessible only within a defined and restricted area, so any bugs affecting that data must be limited to a specific module or layer.

## 2.6 List 5 services provided by an operating system, and explain how each creates convenience for users. In which cases would it be impossible for user-level programs to provide these services? Explain your answer.
5 services provided by an OS:
1. Allocating and deallocating memory
    - This provides fast access to programs, and allows a user to run their programs. Without this services, users couldn't really run any programs at all. This would be impossible for a user-level program to provide this service, because memory management is obviously a dangerous task which requires higher elevation (kernel mode)
2. File system manipulation
    - There are many details involved in file creation, deletion, allocation, and naming that users should not have to perform. Blocks of disk space are used by files and need to be tracked. Deleting a file requires removing the file information, freeing the allocated blocks of storage, and removing the file name from the directory structure. Also, protections must also be checked to assure proper file access only to those with permission. User programs could neither enure adherence to these protection methods nor be trusted to allocate only free blocks of storage and deallocate blocks upon file deletion.
3. Program execution
    - The operating system loads the contents of a file into memory and begins its execution. A user-level program could not be trusted to allocate CPU time for the program to run
4. I/O operations
    - Disks, tapes, serial lines, and other devices must be connected to at a very low level. The user only needs to specify the device and the operation to perform on that device, while the operating system will convert that request into device- or controller-specific commands. User-level programs could not be trusted to access I/O devices, as they could potentially steal information they should not have access to, or do other harmful or malicious things.
5. Communications
    - Message passing between 2 systems requires messages to be turned into packets of information, sent to the network controller, transmitted across some communications medium, and reassembled by the destination system. Packet ordering and data correction must take place to ensure that the information remains intact. Here, user programs might not coordinate proper access to the network device, or they may receive packets which are destined for other processes. 
6. Error Detection
    - Error detection happens both at the hardware and at the software level. 
    At the hardware level, all data transfers must be inspected to ensure that the data has not been corrupted in transit, and this could be done with parity bits or some other methods. All data on media must be checked to be sure they haven't changed since they were written to that media. 
    At the software level, media must be checked for data consistency. For instance, software must check whether the number of allocated and unallocated blocks of storage match the total number of storage blocks on the device. There, errors are frequently process-independent (for instance, the corruption of data on a disk), so there must be a global program (the operating system) that handles all types of errors. Also, by having errors processed by the operating system, processes don't need to contain code to catch and correct all of the possible errors on a system. 
