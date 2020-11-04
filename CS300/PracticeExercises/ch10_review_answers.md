# Chapter 10 Review Answers

## 10.1
### True or False? A program does not need to be stored in memory in its entirety.
- True

## 10.2
### True or False? A physical address space is at least as large as a virtual address space.
- False

## 10.3
### When does a page fault occur?
- A page fault occurs when a requested page is not currently stored in physical memory

## 10.4
### True or False? In a pure demand paged system a page is never brought into memory until it is needed.
- True

## 10.5
### What system call initiates copy on write?
- fork()

## 10.6
### True or False? The vfork() system call does not use copy on write.
- True. In vfork(), both the child and parent process share the same address space, **even after a write**. Because of this, after a vfork() call, the parent process is suspended in its execution until the child process terminates. This is unlike copy-on-write, which allows both processes to run in the same address as well, but the moment that a write is done, the child proecss is copied to its own address space.

## 10.7
### What is the simplest page replacement algorithm?
- The simplest algorithm for page replacement is FIFO. In this algorithm, the OS keeps track of all of the pages in memory in a queue, and the oldest page is in the front of the queue. When a page is to be replaced, the page at the front of the queue is popped off and selected for replacement, while the new page in its place is placed at the rear of the queue.

## 10.8
### What is the name of the page replacement algorithm that operates by replacing the page that will not be used for the longest period of time?
- This is referred to as OPT or MIN in the literature. However, this is difficult to impplement in practice, as it requires future knowledge of the reference string, or set of memory references. An approximation of this is the ```LRU``` or "Least Recently Used" algorithm. This LRU algorithm works by assuming that the process which is least recently used will not be used for the longest period of time (in other words, the past is a good predictor of the future).

## 10.9
### What page replacement algorithm could be implemented using a stack or counters?
- LRU (Least Recently Used). 
- **Counters** could be used by associating with each page-table entry a time-of-use field and add to the CPU a logical clock or counter. The clock is incremented for each memory reference. Whenever a reference to a particular page is made, the contents of the clock register are copied to the time-of-use field for that particular process in the page-table entry for that page. In that way, we always know the most recent time that a particular page-table entry has been referenced. We then replace the page with the smallest time value. This requires a search of the entire page table to find the LRU page and a write to memory (in the copying of the clock register to the page-table entry's time-of-use field) for each memory access. These times also need to be maintained when the page tables are changed due to CPU scheduling. Also, overflow of the clock needs consideration.
- A **stack** could be implemented by keeping a stack of page numbers. Then, any time a page is referenced, it is removed from the stack and placed at the top. Because entries need to be removed from anywhere within the stack, this would need to be implemented by something like a doubly linked list with a head and tail pointer.

## 10.10
### True of False? Approximation algorithms are almost always used when implementing LRU.
- True

## 10.11
### What is the fundamental difference between global and local page replacement?
- In global page replacement, the pool of available frames to be replaced is all of the frames in physical memory, including those currently belonging to other processes. In local page replacement, eligible pages to be replaced are only the pages belonging to each process. For example, if process A needs a new frame of memory, it must replace another page within that same process.

## 10.12
### What term is used to describe the situation where a process spends more time paging tha n executing?
- Thrashing

## 10.13
### What term is used to describe the set of pages a process is currently referencing?
- Working set

## 10.14
### True or False? With pure demand paging, the page fault rate is initially very high.
- True! Because none of the pages for a process are initially in memory, each initial page access will initially reslut in page faults, thereby causing a high page fault rate.

## 10.15
### True or False? Shared memory is typically not implemented using memory mapping.
- False. Shared memory is often implemented using memory mapping.

## 10.16
### Using the buddy system, if a request for 200 KB of kernel memory is made, how much is actually allocated?
- In the buddy system, memoery is allocated in powers of 2. The smallest power-of-2 that 200 KB can fit into is 256 KB, so 256 KB is allocated. In this case, ``` 200 / 256 =~ 0.7812 ``` of the memory allocated is used, so the percentage of internal fragmentation of this particular allocation is ```1.000 - 0.7812 = 0.2188 ```, or about 22% internal fragmentation.

## 10.17
### What is one benefit of using slab allocation?
- One benefit of slab allocation is that slab allocation incurs no internal fragmentation. Because each unique kernel data structure has an associated cache, the exact amount of memory that is needed is the amount of memory that is allocated. 
- Another benefit of slab allocation is speed: frequent allocation and deallocation is particularly fast with slab allocation.

## 10.18
### What is the TLB reach of a system with 4 KB page sizes and 32 entries in the TLB?
- ``` tlb_reach = page_size * tlb_size ```
- ``` page_size = 32 ```
- ``` page_size = 4096 ```
- ``` tlb_reach = 4096 * 32 ```
- ``` tlb_reach = 131072 ```
- 131072 bytes = 128 KiB

## 10.19
### True or False? 4 KB is a typical page size.
- True. 4 KB is perhaps the most common page size.

## 10.20
### True of False? Some systems support page sizes up to 4 MB.
- True. In fact, some systems support even larger page sizes up to 1 GiB

## 10.21
### What page replacement algorithm is used by Windows?
- Primarily, the clock algorithm is used by Windows for page replacement

## 10.22
### Solaris uses the clock algorithm variation of LRU. How many hands does this algorithm employ?
- Solaris uses two hands instead of 1.