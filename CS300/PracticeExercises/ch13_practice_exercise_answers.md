# Chapter 13 Practice Exercise Answers

# (Chapter 11 in Operating Systems - 9E)

## 11.1
### Some systems automatically delete all user files when a user logs off or a job terminates, unless the user explicitly requests that they be kept. Other systems keep all files unless the user explicitly deletes them. Discuss the relative merits of each approach.


## 11.4
### Could you simulate a multilevel directory structure with a single-level directory structure in which arbitrarily long names can be used? If your answer is yes, explain how you can do so, and contrast this scheme with the multilevel directory scheme. If your answer is no, explain what prevents your simulation’s success. How would your answer change if file names were limited to seven characters?
- My answer to this would simply be no. One could attempt to simply use what would be UNIX-style path names wherein each volume, folder, and file is listed as a name separated by the `/` character (e.g. `/home/desktop/research/datasets/coco_dataset.tfrecord`). In this way, saving the file `desktop/test_file.c` would simply be saved as a file named `desktop/test_file.c`. However, protection would be incredibly unwieldy. If I wanted to prevent access to the `desktop` directory for users, I couldn't do so with this scheme, unless I simply edited each file individually which matches the string `desktop` exactly, which is essentially just simulating directory structure. If file names were limited to 7 characters, than this simulation would certainly not work.

## 11.5
### Explain the purpose of `open()` and `close()` operations.
- The `open()` system call informs the system that the named file is about to become active. After issuing an `open()` operation, the system searches the directory for the file, copying the directory entry into the open-file table. Then, the function returns a pointer to that entry in the open-file table. This way, information about the file can be maintained, and the entire directory structure need not be searched each time the file is accessed within the process calling `open()`.
- The `close()` system call informs the system that the named file is no longer being actively used by the user who issued the `close` operation.

## 11.6
### In some systems, a subdirectory can be read and written by an authorized user, just as ordinary files can be.
### a. Describe the protection problems that could arise.
### b. Suggest a scheme for dealing with each of these protection problems.
- One piece of information kept in a directory entry is file location. If a user was allowed to modify this location, then he/she could access other files defeating the access-protection scheme.
- Don't allow the user to write directly to the subdirectory. Rather, provide system operations to do so.

## 11.7
### Consider a system that supports 5,000 users. Suppose that you want to allow 4,990 of these users to be able to access one file.
### - How would you specify this protection scheme in UNIX?
### - Can you suggest another protection scheme that can be used more effectively for this purpose than the scheme provided by UNIX?
- There are 2 methods for achieving this:
  - Create an access control list with the names of all 4990 users
  - Put these 4990 users into one group and set the group access accordingly. This scheme can not always be implemented since user groups are restricted by the system.
- The universal access to files applies to all users unless their name appears in the access-control list with different access permission. With this scheme you simply put the names of the remaining ten users in the access control list but with no access privileges allowed.

## 11.8
### Researchers have suggested that, instead of having an access list associated with each file (specifying which users can access the file, and how), we should have a user control list associated with each user (specifying which files a user can access, and how). Discuss the relative merits of these two schemes.
- *File Control List*
  - Because the access control information is concentrated in a single location, it is easier to change access control information and requires less space.
- *User Control List*
  - This requires less overhead when opening a file, as the only computation necessary is checking whether the given file and respective permissions are present in the control list for the prompting user.

## 11.9
### Consider a file system in which a file can be deleted and its disk space reclaimed while links to that file still exist. What problems may occur if a new file is created in the same storage area or with the same absolute path name? How can these problems be avoided?
- Obviously, this could lead to many undefined behaviors. Firstly, a program could attempt changes to the original file, unknowingly making changes to the new file in its place, which could lead to all sorts of terrible things. Secondly, if a user had access to the first file, and thus the link to the first file, but they aren't supposed to have access to the second file, then they might be able to access the second file anyways if the system is unaware that the link is pointing to a new file. This can be avoided by ensuring that all links to a deleted file are also deleted. This could be accomplished in several ways:
  - Maintain a list of all links to a file, removing each of them when the file is deleted.
  - Retain the links, removing them when an attempt is made to access a deleted file
  - Maintain a file reference list (or counter), deleting the file only after all links or references have been deleted

## 11.10
### The open-file table is used to maintain information about files that are currently open. Should the operating system maintain a separate table for each user or maintain just one table that contains references to files that are currently being accessed by all users? If the same file is being accessed by two different programs or users, should there be separate entries in the open-file table? Explain.
- By keeping a central open-file table, the system can perform the following operation that would be infeasible otherwise. Consider that a file is currently being accessed by one or more processes. If the file is deleted, it shouldn't be removed from the open-file table until all processes acessing the file have closed it. This check could be performed only if there was central accounting of the number of processes accessing the file. On the other hand, if two processes are accessing the file, then separate states need to be maintained to keep track of the current location of which parts of the file are being accessed by the two processes. This requires the operating system to maintain separate entries for the two processes. In fact, this could be done by having each process store its own open-file table which simply points to the global open-file table and keeps track of other data such as file pointer location in its own private open-file table.

## 11.11
### What are the advantages and disadvantages of providing mandatory locks instead of advisory locks whose use is left to users’ discretion?
- If the lock is mandatory, then once a process acquires an exclusive lock on a file, the OS will prevent any other process from accessing that file. As a benefit, this means that the process does not have to worry about badly coded applications and programs manipulating, writing, or reading to the files it's working on while it's using those files. It also potentially simplifies the creation of programs from the programmer's perspective, as they don't need to worry about accidentally manipulating files that are locked by other processes. However, mandatory locks limit the flexibility of the system and what programs can do with concurrent access to files. If I wanted to create some program which involved multiple processes writing to a single file in some complex way, I could do so in a system with advisory locking, as the locking is left up to the discretion of the developer. In a mandatory locking system, I could not do so (atleast not without obfuscated, non-obvious ways of getting around the mandatory lock system).

## 11.13
### Some systems automatically open a file when it is referenced for the first time and close the file when the job terminates. Discuss the advantages and disadvantages of this scheme compared with the more traditional cone, where the user has to open and close the file explicitly.
- Automatic opening and closing of files relieves the user from the invocation of these functions, and thus makes file manipulation more convenient for the user in this sense. However, this requires more overhead than the case where explicit opening and closing is required.

## 11.17
### Some systems provide file sharing by maintaining a single copy of a file. Other systems maintain several copies, one for each of the users sharing the file. Discuss the relative merits of each approach.
- With a single file, several concurrent updates may result in the user obtaining incorrect information, and the file being left in an incorrect state. As can be seen, this is a situation in which any of the strategy for magaing concurrency would come into play (mutual exclusion, locking, semaphores, etc.). With multiple copies, there is lots of wasted space due to duplicate information and the various copies may not be consistent with eachother.
