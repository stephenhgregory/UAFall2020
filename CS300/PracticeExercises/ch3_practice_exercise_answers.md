# Chapter 3 Practice Exercise Answers

## 3.1
### Using the program shown in Figure 3.30, explain what the output will be at LINE A. 
Line A is inside of an if statement that will only be evaluated to true if the process pid > 0. In this case, after the ```fork()``` call, the value of the variable ```pid``` will be 0 in the child process, so this if statement will **not** evaluate to true for the child process. In the parent process, however, the value of the variable ```pid``` which is returned by ```fork()``` will be equal to the PID of the newly created child process. I will say again, because it's a bit confusing: the variable "pid" in the parent will **not** be equal to the PID of the parent process, it will be equal to the PID of the new child process. Therefore, Line A will only be executed in the parent process, and the value of the variable named ```value``` will still retain a value of 5, even after the child process is terminated, and the parent process finishes "waiting".

## 3.2
### Including the initial parent process, how many processes are created by the program shown in Figure 3.31?
Because there are 3 consecutive calls to ```fork()```, a process spawns a child process each time. The first call is run by one process which creates one new child process, giving us 2 total processes and 1 'new' process. The second call is run by both of those processes, each of which create one new child process, giving us 4 total processes and 3 'new' process. The third call is run by all 4 of these processes, each of which create 1 new child process, giving us 8 total processes and 7 'new' processes. Therefore, there are 7 new processes created by the function, and 8 processes total, including the first process.

## 3.3
### Original versions of Apple's iOS operating system provided no means of concurrent processing. Discuss three major complications that concurrent processing adds to an operating system.
1. Process communication complexity
    - Adding concurrent processing to an operating system means that there must be some method of communication between processes. Much of Chapter 3 is dedicated to explaining this issue of process communication in detail. In short, there are many ways of creating effective process communication such as shared memory and message passing, all of which involve creating non-trivial solutions to the issue of process communication. All of these methods take time to develop, add complexity to the operating system, and demand more of the computer hardware when compared to a simple, non-multi-tasking system.
2. Security
    - Adding concurrent processing means that user applications have the capability to interact and interfere with other user applications, which opens the door to security threats from malicious or bad-behaving applications. The original versions of iOS did not provide a means of concurrent processing for user applications downloaded from the App Store specifically for this reason.
3. Swapping programs in and out of memory
    - If a running process requires a large block of memory, then some other processes may need to be dumped back to secondary storage. This leads to overhead!
4. Context switching overhead
    - When the CPU needs to switch from one process to another process, it must **switch** context, which is pure overhead (the CPU does no useful work during this time). In a context switch, the CPU must save the current process state into the current process' Process Control Block (PCB). Then the CPU must restore the state of the other process that it is switching to using the PCB of that other process.
5. Memory Management
    - The OS has to keep track of the main memory address space allocated to each process so that a process does not affect or harm any other processes. This is vital, as one process may be a virus, and might try to alter the address space of other processes in memory, which can't be allowed to happen!

## 3.4
### Some computer systems provide multiple register sets. Describe what happens when a context switch occurs if the new context is already loaded into one of the register sets. What happens if a new context is in memory rather than a register set and all of the register sets are in use?
If the new context is already loaded into one of the register sets, the CPU current-register-set pointer is simply changed to point to the register set containing the new context. There is very little overhead here. However, if a new context is in memory, and all register sets are in use, then one of the contexts in a register set must be chosen to be moved back into memory to allocate space for the new context. This case involves almost the same overhead as a system without multiple register sets, wherein the context must be fully copied into the register set. The only thing different between a multiple-register system and single-register system, at this point, is that the current-register-set pointer must now be updated to point to the register set with the new context on our multi-register system.

## 3.5
### When a process creates a new process using the fork() operation, which of the following states is shared between the parent process and the child process? 
### - Stack
### - Heap 
### - Shared memory segments
Only the shared memory segments are shared between the parent process and the newly created child process. Copies of the stack and heap are made for the newly created process.

## 3.6
### Consider the “exactly once” semantic with respect to the RPC mechanism. Does the algorithm for implementing this semantic execute correctly even if the ACK message sent back to the client is lost due to a network problem? Describe the sequence of messages, and discuss whether “exactly once” is still preserved. 
The "exactly-once" semantics ensure that a remote procedure will be performed once and only once. The general algorithm for implementing this involves an acknowledgement (ACK) scheme combined with timestamps or some other incremental counter that allows the server to distinguis between duplicate messages.
The general strategy is for a client to send an RPC to the server along with a timestamp. The client will also start a timeout clock. Then, the client will wait for one of two occurrences: (1) it will receive an ACK from the server indicating that the procedure was performed, or (2) it will time out. If the client times out, it assumes the server was unable to perform the remote procedure, so the client invokes the RPC a second time, sending a later timestamp. The client may not receive the ACK for one of 2 reasons: (1) the original RPC was never actually received from the server, or (2), the RPC was correctly received - and performed - by the server but the ACK was lost. In situation (1), the ideal situation, the use of ACKS allows the receiver ultimately to receive and perform the RPC. In situation (2), the server will receive a duplicate RPC and it will use the timestamp to identify it as a duplicate so as not to perform the RPC a second time. It is important to note that the server must send a second ACK back to the client to inform the client that the RPC has been performed.

## 3.7
### Assume that a distributed system is susceptible to server failure. What mechanisms would be required to guarantee the “exactly once” semantic for execution of RPCs? 
Firstly, the server should keep logs in non-volatile storage regarding what RPC operations were received, whether they were successfully performed, and the results associated with the operations. When a server crash takes place and an RPC message is received, the server can check whether the RPC has been performed and therefore guarentee "exactly once" semantics for the execution of RPCs.