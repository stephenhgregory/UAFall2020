# Chapter 1 Practice Exercise Answers

## 1.1.
The 3 main purposes of an operating system are:
1. To provide an environment for a user to execute programs on computer hardware in a convenient and efficient manner
2. To allocate the separate resources of the computer as needed to solve any given problems. This allocation should be done in a fair and efficient way.
3. To serve as a control program that serves 2 major functions:
    - Supervision of the execution of user programs to prevent errors, improper/malicious use of computer
    - Management of operation and control of I/O devices.
			
## 1.2.
Sometimes, it may be appropriate to 'waste' resources in a single-user system if it makes the user experience smoother or easier. This isn't truly wasteful, because those resources are better allocated to serving the user a better user experience, rather than having the system use all of its resources completely "efficiently" but slower, clunkier, or with a worse overall experience to the user. Also, single-user systems typically require less resources than multi-user systems.
	
## 1.3
The main difficulty in writing an OS for a real-time environment is that processes MUST be done within a certain time-frame, or they are as good as useless. Therefore, one must build-in the proper framework to handle situations when the specified task cannot be completed within the time frame. In many cases, a real-time OS may need to be very cautious and abort ALL action if the desired action cannot be executed in time. In implementation, this means that the programmer must pay lots of attention to resource allocation and optimization.
	

## 1.4
### First, an argument that an operating system should **not** include applications like browsers and mail programs:
An operating system should be designed from the ground up to provide a framework for users to interact with the computer's hardware, as well as a space for applications to live and be interacted with. In this definition of an OS, having applications like mail and web browsers are not part of an OS; rather, they are a part of applications which live on top of the OS. Put this way, are programs like a web browser and email client vital to the functionality of the computer? It could be argued that the answer to that question is no. Therefore, these should not be a part of an operating system, not because they are not immensely useful, but because they do not help the computer function or provide a framework for other applications to work and interact with eachother in any way.
### Second, an argument for the contrary: that an operating system **should** include a browser and email program:
It is undoubtedly true that the primary function of an operating system is to provide a framework that makes it convenient, safe, secure, and simple to access a computer's resources, in addition to the other programs on the computer. However, in the landscape of modern use cases for computers, email clients and web browsers are absolutely necessary. It could be argued that an operating system shipped to a customer should contain all of the functionality needed for the user to use their computer at the most basic level. In 2020, accessing the internet and receiving emails are included in this definition: a computer is **incomplete** if it does not have the capability to do either of these things. Additionally, if the user is expected to download these programs themselves, how are they expected to find and download these programs if they can't access the internet through a web browser? By omitting a web browser specifically, the OS is forcing the user to do one of two things: (1) Purchase a web browser downloaded onto a CD/DVD/Flash Drive, or (2) Use the OS's proprietary "app-store" (if one is even available on the OS). Not to mention, if the user somehow accessed the internet via a different device, and sent an application download URL to their email, they would have no way of accessing that download URL because they wouldn't have an email client installed on their system!

## 1.5
The 'mode' bit which allows distinction between kernel and user modes adds a layer of security in that a user is prevented from running any programs/processes that they want. Many programs can only be run by the OS, and the additional layer of abstraction gained from requiring that a user first call to the OS *asking* if it can run a certain process adds a level of security. By disallowing users from EVER running certain critical applications directly, it is easy to (a) stop a user from inadvertently or maliciously running harmful or incorrect processes, or even just processes at the wrong time, and it is easy to (b) isolate security breaches if they do happen, by narrowing the path of entry to the execution of those critical processes.

## 1.6
1. Set value of timer. **Should be privileged**
    - This is an important part of the system that should be protected.
2. Read the clock. No need to be privileged
    - This isn't a dangerous operation, so there's no need to protect it.
3. Clear memory. **Should be privileged**
    - This is a dangerous operation that may be fatal, so it should be protected.
4. Issue a trap instruction No need to be privileged
    - Everyone is allowed to issue a trap instruction. In fact, traps are often the way that user programs make system calls.
5. Turn off interrupts. **Should be privileged**
    - Interrupts are a vital part of the computer system, so turning them off could be catastrophic.
6. Modify entries in device-status table. **Should be privileged**
    - This is another dangerous operation, so should be protected.
7. Switch from user to kernel mode. **Should be privileged**
    - This is very dangerous. The security afforded by the kernel mode bit would be nullified if anyone could switch to kernel mode, because then, user programs could pose as the operating system and execute any privileged instruction that they wanted. This is obviously dangerous.
8. Access I/O device. **Should be privileged**
    - Accessing I/O devices should be controlled, so this should be protected.

## 1.7
There are difficulties associated with the practice adopted by early computers of placing the OS in a memory partition that could not be modified by either the user job or the operating system itself.
1. Such an OS could not ben modified or updated at all. Any bug fixes could not be performed, since there is no way to change the OS on its partition of memory
2. All passwords and other credentials must be stored somewhere in unprotected memory, since they could not be stored with the unmodifiable protected memory where the OS is stored.

## 1.8
Some CPUs have more than 2 modes of operation. This could be used in a multitude of ways. For example, level 0 may be "kernel", which is the level at which any instructions available to the computer can be run, and level 1 and level 2 may be "stock application", which is the level afforded to applications shipped with the operating system. Then perhaps level 3 could be "user", which may be the level afforded to all 3rd party applications.

## 1.9
A program could use timers to compute the current time by setting a timer for some tim ein the future, and then goingg to sleep. When awakened by the interrupt, it could update its local state, which its using to keep track of the number of interrupts its received so far, and then go to sleep again. It could simply repeat this process of continually setting timer interrupts and updating its local state when the interrupts are actually raised.

## 1.10
### 2 reasons why caches are useful
1. Caches provide the CPU with a faster method of retrieving data
2. Caches reduce unnecessarily accessing the same memory over and over again
Caches introduce issues associated with having data in even more places. For example, a variable x may be in cache, in a register, and memory, AND in long-term storage. This simply leads to increased complexity associated with making sure that all instances of the variable are equivalent. 
Unfortunately, caches cannot be as large as permanent storage for several reasons. Firstly, cache memory is far more expensive per-bit than something like a hard disk drive, due to the nature of its physical structure and the subsequent production requirements. Secondly, cache memory must be physically very close to the SPU's processing core. If a cache as big as a 1TB HDD were in a computer case, it would be a minimum of a few inches away from the CPU core, which is significantly larger than the distance between current caches and processing cores. Finally, cache memory is volatile, meaning that data is only held onto while the device is on and receiving power. Everytime the computer is turned off, the cache is reset. Therefore, nothing could be stored on the computer permanently!

## 1.11
### Client-Server vs peer-to-peer models of distributed systems
In a client-server architecture, client machines are always client machines, and they always communicate up to a central server machine, which serves out files, computing resources, or even entire applications and UIs to interact with those applications. Therefore, two client machines are just that -clients- and cannot serve data directly to one another. In contrast, a peer-to-peer architecture is one in which all of the machines in the distributed system may be clients and servers and different times. In this architecture, any machine may act as a server if it needs to send files to another machine, which would act as the client. Likewise, a machine that wishes to receive some files, software, or anything else would act as a client and would simply ask another machine for such data.

## 1.12
### How do clustered systems differ from multiprocessor systems?
A clustered system is typically many separate systems (each systeam being a server, desktop computer, or any other fully independent computer) which communicate together through some common medium. This could mean that all systems work share storage, or perform cooperative computation on the same data, but the systems are fully separate. A multiprocessor system is a single computer containing multiple processors. In this system, RAM, I/O, storage, the motherboard, and other hardware are shared, while there is simply multiple CPUs performing computation. Therefore, a multiprocessor system is much more tightly coupled than a clustered system.
### What's required for 2 machines belonging to a cluster to cooperate to provide a highly available service?
Clustered systems could interact using some messages in a specific networking or clustering protocol, while multiprocessor systems may share memory to cooperate. In a NUMA system, each processor has its own local memory, so when communication is required between multiple processor-memory pairs (called NUMA nodes), a separate communication protocol must be used. In order for two machines to provide a highly available service, a state that exists on both machines should be replicated and consistently updated. Then, when one of the machines fails, the other machine can take over the functionality of the failed machine. This gives us redundancy!

## 1.13
### Consider a computing cluster consisting of 2 nodes running a database. Describe 2 ways in which the cluster software can manage access to the data on the disk, and discuss the benefits of each.
We can use two types of clustering:
1. Asymmetric clustering
    - One machine is in 'hot-standby' mode, and the other machine is the active server running the database. In this mode, the hot-standby machine has a running copy of the database that is constantly updated to match the active server. However, the hot-standby machine isn't doing anything; it's only monitoring the active server. That way, if the active server goes down for any reason, the hot-standby machine will fill its shoes and become the active server. This way, if the active server fails, the whole system might only be down for a few seconds or minutes and not hours or days!
    - **Benefits:**
        - Safety - Redundancy means that the threat of data loss is reduced
        - Simplicity - There is no complex communication between machines in the cluster - The standby machine simply copies the active server until the active server goes down (one-way communication)
    - **Drawbacks:**
        - Inefficiency - The machine that is not the active server is simply sitting there, wasting computing resources
        - Speed - This cluster is no faster than a single machine
2. Symmetric clustering
    - Both machines are running at the system, and are monitoring eachother. In our case, one machine could be running half of the tables and all of the stored procedures, triggers, etc related to that half of the database, and the other machine could be running the other half of the tables and the rest of the stored procedures, triggers, etc. However, chances are, the two 'halves' of the database would need to talk to eachother, so each server would frequently need to communicate with the other.
    - **Benefits:**
        - Efficiency - Both machines are being utilized at all times, so there is no waste of compute time or resources
        - Speed - The cluster can deliver faster performance, in this case faster queries, because there are multiple machines available to service clients
    - **Drawbacks:**
        - Complexity - The implementation of parallel clustering is difficult, and we must use special software, or do it ourselves.
        - No/less redundancy - The cluster may not benefit from full redundancy - If one machine goes down, there may be data loss, and it may take a long time to get the server up and running again.

## 1.14
### What's the purpose of interrupts? How does an interrupt differ from a trap? Can traps be generated intentionally by a user program? If so, for what purpose?
Interrupts are hardware-generated change-og-flows used to send signals to the CPU that a service is needed urgently. They literally "interrupt" the CPU so that it will drop what it's doing and summon an interrupt handler to deal with the cause of the interrupt (solve that problem). A trap is simply a software-generated interrupt. 

## 1.15
### Explain how the Linux kernel variables HZ and jiffies can be used to determine the number of seconds the system has been running since it was booted
Jiffies is a Linux kernel variable which tells the user the number of timer interrupts that have occurred since system boot up. HZ tells the user the frequency of timer interrupts, or in other words, the number of timer interrupts per second. Therefore, to find the total number of seconds that the system has been turned on, one can simply divide the value stored in the jiffies variable with the value stored in the HZ variable. 
```
jiffies / HZ = seconds
```

## 1.16
### Director Memory Access (DMA) is used for high-speed IO devices in order to avoid increasing the CPU's execution load. 
1. How does the CPU interface with the device to coordinate the transfer?
  - To initiate a DMA transfer, the CPU first sets up the DMA registers, which contain a pointer to the source of a transfer, a pointer to the destination of a transfer, and a counter of the number of bytes to be transferred. Then, the DMA controller proceeds to place addresses on the bus to perform transfers, while the CPU is free to do other work.
2. How does the CPU know when the memory operations are complete
  - Once the entire transfer is complete, the DMA controller sends an interrupt to the CPU signifying that it is done.
3. The CPU is allowed to execute other programs while the DMA controller is transferring data. Does this process interfere with the execution of the user programs? If so, describe what forms of interference are caused. 
  - Both the CPU **and** the DMA are what is called *bus masters*, meaning that both of them can control the system bus. Therefore, there would be a problem if both the CPU and the DMA controller wanted to access the memory at the same time. Therefore, the CPU should be momentarily prevented from accessing main memory when the DMA seizes the memory bus. However, if the CPU is still able to access data in its primary and secondary caches, a coherency issue may be created if both the CPU and DMA controller update the same memory locations.

## 1.17
### Some computer systems do not provide a privileged mode of operation in hardware. Is it possible to construct a secure operating system for these computer systems? Give arguments both that it is and that it is not possible.
First, we should define security. By secure, we mean that a user program is not able to corrupt the kernel, crash the system, violate memory protection, etc. 

There are several approaches that could be taken. Firstly, one could write a simulator for a processor that does indeed have a kernel mode bit, and running the entire operating system on top of this emulator. However, this is slow. Another variant of this approach would be to run the operating system normally, but to run all user programs only in an emulator (This is the approach used in [Nachos](https://en.wikipedia.org/wiki/Not_Another_Completely_Heuristic_Operating_System)). 

Another approach is to rely on safe languages. Only executing user programs written in a 'safe' language such as Java will work because the properties of the language guarantee the manual, arbitrary writes to memory are not allowed and there is a limited instruction set. Note here that the user doesn't get to choose arbitrary machine code to run - the code that is run is the result of just-in-time compilation of the Java source code. Similar to this approach is running only code which has been produced by a trusted compilter that is known to check memory addresses and not output priviliged instructions.

It is **not** enough to have the kernel scan the user code for bad instructions before executing it, if the code might have been produced by an arbitrary compiler. Firstly, bad instructions are hard to spot - for example, they might be a write instruction which happens to overwrite critical data, but which is not easily identifiable as doing such. Directly executing user code is also dangerous because the user code could be doing something indirect like overwriting itself or writing new code to a new page of memory and then jumping to that new page in memory, and that code would then not be checked for security.

Similarly, asking the processor to check with the OS before executing each instruction doesn't quite work. Processor don't physically have a mode like this, and if if one did, that would basically be the same as having amode bit - there needs to be a way for the processor to know to check user code but not the OS itself. To make this work, you could use a full emulator, as described previously.

## 1.18
### Many SMP systems have different levels of caches; one level is local to each processing core, and another level is shared among all processing cores. Why are caching systems designed this way?
One of the main reasons for this design is speed. Local caches are much faster than shared caches. The data that is expected to be used by some processor is put into its local cache, but is also put into the shared cache *if it is not already there* (notice that it is a plausible that it was in the shared cache before it was transferred to the local cache). We put the data into the shared cache so that it can be quickly transferred to the local cache of another processor if its starts some new process which needs this data. Without the shared cache, this operation would be much slower as each processing core would have to pull everything from main memory to get data into its own local cache

## 1.19 
### Rank the following storage systems from slowest to fastest
1. (slowest) Magnetic tapes
2. Optical disk
3. Hard-disk drives
4. Nonvolatile memory
5. Main memory
6. Cache
7. Registers

## 1.20
### Consider an SMP system. Illustrate with an example how data residing in memory could in fact have a different value in each of the local caches.
We'll say that processor 1 reads data A with a value of 5 from main memory into its local cache. Similarly, processor 2 reads data ! into lits local cache as well. Process 1 then updates A to 10. However, since A resides in processor 1's local cache, the update only occurs there, in processor 1's local cache, and not in the local cache for processor 2.

## 1.21
### Discuss, with examples, how the problem of maintaining coherence of cached data manifests itself in the following processing environments:
1. Single-processor systems
2. Multiprocessor systems
3. Distributed systems

1. In a single-processor system, separate local caches must synchronize with eachother by synchronizing themselves with a shared cache