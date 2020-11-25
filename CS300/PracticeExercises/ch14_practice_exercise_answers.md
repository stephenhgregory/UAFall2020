# Chapter 14 Review Answers

## 14.1
### Consider a file currently consisting of 100 blocks. Assume that the file-control block (and the index block, in the case of indexed allocation) is already in memory. Calculate how many disk I/O operations are required for contiguous, linked, and indexed (single-level) allocation strategies, if, for one block, the following conditions hold. In the contiguous-allocation case, assume that there is no room to grow at the beginning but there is room to grow at the end. Also assume that the block information to be added is stored in memory.
### (a) The block is added at the beginning.
### (b) The block is added in the middle.
### (c) The block is added at the end.
### (d) The block is removed from the beginning.
### (e) The block is removed from the middle.
### (f) The block is removed from the end.
- (a) Added at the beginning: 
  - Contiguous: 101 IO operations ?
  - Linked: 1 IO operation ?
  - Indexed: 2 IO operations ?
- (b) Added at the middle:
  - Contiguous: 101 IO operations ?
  - Linked: 51 IO operations ?
  - Indexed: 2 IO operations ?
- (c) Added at the end:
  - Contiguous: 1 IO operations ?
  - Linked: 101 IO operations ?
  - Indexed: 2 IO operations ?
- (d) Removed from the beginning: 
  - Contiguous: 1 IO operation ?
  - Linked: 1 IO operation ?
  - Indexed: 2 IO operations ?
- (e) Removed from the middle:
  - Contiguous: 101 IO operations ?
  - Linked: 51 IO operations ?
  - Indexed: 1 IO operations ?
- (f) Removed from the end:
  - Contiguous: 1 IO operations ?
  - Linked: 101 IO operations ?
  - Indexed: 1 IO operation ?

## 14.2
### Why must the bit map for file allocation be kept on mass storage, rather than in main memory?
- The bit map for file allocation is needed to maintain information about the file system. Because files and the file system persist (they keep their state even when the system is powered off), the method we use to keep track of the file system must also persist. For this reason, the bit map must be stored in mass storage rather than main memory, becase mass storage persists over time, and main memory is not used for those purposes (it is *volatile*).

## 14.3
### Consider a system that supports the strategies of contiguous, linked, and indexed allocation. What criteria should be used in deciding which strategy is best utilized for a particular file? 
- Firstly, the type of access to the file is perhaps the most important criteria for deciding a file allocation strategy. For random-access situations, linked allocation is a very poor choice, as each block must be read sequentially in order to find the proper file, whereas indexed and contiguous allocation can be used for fast random access. For sequential access, either allocation scheme works well. Other important factors when choosing file allocation strategy are file size and read/write frequency. Indexed allocation incurs overhead for the storage of the index block, so small files only taking a few blocks might be a poor choice for indexed allocation, as the relative waste of storage is greater for index blocks who point to fewer blocks.

## 14.4
### One problem with contiguous allocation is that the user must preallocate enough space for each file. If the file grows to be larger than the space allocated for it, special actions must be taken. One solution to this problem is to define a file structure consisting of an initial contiguous area of a specified size. If this area is filled, the operating system automatically defines an overflow area that is linked to the initial contiguous area. If the overflow area is filled, another overflow area is allocated. Compare this implementation of a file with the standard contiguous and linked implementations. 
- This method requires more overhead than the standard contiguous allocation scheme, but it requires less overhead than linked allocation. This seems somewhat similar to extents.

## 14.5
### How do caches improve performance? Why do systems not use more or larger caches if they are so useful?
- Caches allow components of different speeds to communicate more efficiently by storing data from the slower device, temporarily, in a faster device (cache). Caches are, almost by definition, more expensive than the device that they're caching for, so increasing the number or size of caches would increase system cost.

## 14.6
### Why is it advantageous to the user for an operating system to dynamically allocate its internal tables? What are the penalties to the operating system for doing so? 
- Dynamic tables allow for more flexibility in system use growth - tables are never exceeded, avoiding artificial use limits. Unfortunately, kernel structures and code are therefore more complicated, so there is more potential for bugs. The use of one resource can take away more system resources (by growing to accomodate the requests) than with static tables.

## 14.7
### Consider a file system that uses a modified contiguous-allocation scheme with support for extents. A file is a collection of extents, with each extent corresponding to a contiguous set of blocks. A key issue in such systems is the degree of variability in the size of the extents.  What are the advantages and disadvantages of the following schemes? 
### (a) All extents are of the same size, and the size.
### (b) Extents can be of any size and are allocated dynamically
### (c) Extents can be of a few fixed sizes, and these sizes are predetermined.
- If all extents are of the same size, and the size is predetermined, then it simplifies the block allocation scheme. A simple bit map scheme or free list for extents would suffice. If the extents can be of any size and are allocated dynamically, then more complex allocation schemes are required. It might be difficult to find an extent of the appropriate size and there might be external fragmentation. One could use the buddy system allocator discussed previously to design an appropriate allocator. When the extents can be of a few fixed sizes, and these sizes are predetermined, one would have to maintain a separate bitmap or free list for each possible size. This scheme is of intermediate complexity and of intermediate flexibility in comparison to the earlier schemes.

## 14.8
### Contrast the performance of the three techniques for allocating disk blocks (contiguous, linked, and indexed) for both sequential and random file access. 
- For sequential access, contiguous allocation is great, and sequential access is also good. (It might not be quite as good as contiguous, becuase the blocks can be scattered across the storage medium, and the possibility of reading in a large chunk of blocks in the same location is no longer guaranteed as with contiguous allocation). Also, indexed allocation is not fantastic for sequential access, because the index block needs to be constantly referenced for each new block and the blocks may be very scattered. For random access, contiguous allocation performs great, as does indexed allocation, but linked allocation is very poor. Because linked allocation is essentially a linked list of blocks, blocks must be traversed from the very first block to the desired block, which incurs lots of IO and is quite inefficient.

## 14.9
### What are the advantages of the variant of linked allocation that uses a FAT to chain together the blocks of a file? 
- For a process attempting to randomly access a file, that process does not need to read in every single block of data for the file. Rather, the process can simply read the portion of the FAT corresponding to the file, and scan through the block pointers to find the correct block. In contrast, the typical implementation of linked allocation involves the last portion of every block allocated to a pointer to the next block, which requires that a process read in the entire block of data to access the very small portion of the block that is needed (the next-block pointer).

## 14.10
### Consider a system where free space is kept in a free-space list.
### (a) Suppose that a pointer to the free-space list is lost. Can the system reconstruct the free-space list? Explain your answer.
### (b) Consider a file system similar to the one used by UNIX with indexed allocation. How many disk I/O operations might be required to read the contents of a small local file at /a/b/c? Assume that none of the disk blocks is currently being cached. 
### (c) Suggest a scheme to ensure that the pointer is never lost as a result of memory failure.
- (a)
  - Yes, but the amount of time required to reconstruct the free-space list will grow linearly with the number of files in the file system. To reconstruct the file system, the OS must scan the file system beginning at the root. As each directory and file is scanned, the OS marks the associated disk blocks as used. (Notice that this includes not only file data, but any file control blocks and index blocks associated with the file, as well as blocks (both data and meta-data) used by directories. In addition, blocks occupied by volume- and boot-control blocks must be accounted for.) One problem with this scheme is that some OSes deal with damaged (unreadable and/or unwritable) disk blocks simply by removing them from the free list. The scanning method used above would identify these blocks as free, since they are not part of the file system structure. The danger is obviously that the information that these blocks are bad is lost, so these blocks may be allocated to a file, even though they are bad. Thus, the OS would need to keep a separate list of these blocks on disk. Early versions of Unix solved this problem by allocating these bad blocks to a special file whose only purpose was to be composed of bad blocks, and placing this file in a special directory. This directory ("/lost+found") still exists on some systems.
- (b)
  - Recall that in Unix, directories are just regular files that contain a list of file/directory names and pointers to FCB's (inodes). In the best case, reading each directory or file requires three disk block accesses: one to read the inode for the file or directory, one to read the index block pointed to by the inode, and then one to read the first data block of the file or directory itself. If we assume to know the address of the root inode when we begin, then `12` reads are required:
    - 1. Read the root inode,
    - 2. Read the index block of the root directory,
    - 3. Read the root directory,
    - 4. Read a's inode,
    - 5. Read a's index block,
    - 6. Read directory a,
    - 7. Read b's inode,
    - 8. Read b's index block,
    - 9. Read directory b,
    - 10. Read c's inode,
    - 11. Read c's index block,
    - 12. Read first block of file c
  - If we assume a double-indirect block scheme, then in the worst case, 4 reads are required to get the appropriate block of a directory or file into memory: read the inode, read the outer indirect block, read the inner indirect block, read the data block. This gives us a total of 16 reads.
- (c)
  - Store the pointer to the free space list in several different locations on disk.
  
## 14.11
### Some file systems allow disk storage to be allocated at different levels of granularity. For instance, a file system could allocate 4 KB of disk space as a single 4-KB block or as eight 512-byte blocks. How could we take advantage of this flexibility to improve performance? What modifications would have to be made to the free-space management scheme in order to support this feature? 
- Such a scheme would decrease internal fragmentation. If a file is 5 KB, then it could be allocated a 4 KB block and two contiguous 512-byte blocks. In addition to maintaining a bitmap of free blocks, one would also have to maintain extra state regarding which of the subblocks are currently being used inside a block. The allocator would then have to examine this extra state to allocate subblocks and coalesce the subblocks to obtain the larger block when all of the subblocks become free.

## 14.12
### Discuss how performance optimizations for file systems might result in difficulties in maintaining the consistency of the systems in the event of computer crashes. 
- One primary difficulty that might arise is due to caching and delayed updates of data/metadata. Updates may be delayed hoping that the same data might be updated in the future or that the updated data may be temporary and may be deleted in the near future. However, if the system crashes without committing the delayed updates, the consistency of the file sytem is destroyed.

## 14.13
### Discuss the advantages and disadvantages of supporting links to files that cross mount points (that is, the file link refers to a file that is stored in a different volume). 
- As a disadvantage, supporting such links may mean that other volumes will need to be mounted at any given moment. The main advantage of this approach is that there may be extra disk space on the other mount point. Because this is usually done with symbolic, or "soft" links, as opposed to hard links, there is an incurred disadvantage that renaming the file will not have the expected effect, or when creating a file with the same name in the same location, the file will be created on the first file system. Also, renaming files across different mount points may fail, e.g. if the file is renamed from the symlinked file to the local directory.

## 14.14
### Consider a file system on a disk that has both logical and physical block sizes of 512 bytes. Assume that the information about each file is already in memory. For each of the three allocation strategies (contiguous, linked, and indexed), answer these questions: 
### 1. How is the logical-to-physical address mapping accomplished in this system? (For the indexed allocation, assume that a file is always less than 512 blocks long.) 
### 2. If we are currently at logical block 10 (the last block accessed was block 10) and want to access logical block 4, how many physical blocks must be read from the disk? 


