# Chapter 4 Practice Exercise Answers

## 4.1
### Provide three programming examples in which multithreading provides better performance than a single-threaded solution
1. Web Browser
    - In a web browser, it makes a lot of sense for us to have multiple threads. Within one single tab, on one webpage, we may have separate threads for user input, HTTP requests, and webpage rendering. That way, we can be posting an HTTP POST request to a server while a video is still playing on the screen. Ruthermore, we could also type in a search bar while the return code for the HTTP POST request is still not received, and the video is still playing. Without multithreading, the the video would have to stop playing while our HTTP POST request is being sent and waited on, and clicking and typing on the search bar wouldn't work. The whole thing would appear to "freeze".
2. Divide and Conquer algorithms
    - In a divide-and-conquer algorithm such as merge-sort, each recursively created subproblem can be created as a separate thread. This is a perfect use-case for multithreading, because each subproblem does not need to wait for the other subproblems to compute. In this case, each thread can run concurrently, as each is an independent unit of work. Potentially, the end of the recursive task-creation could be triggered at a small constant size N. In this way, once subproblems reached a size less than or equal to N, the problem could be solved in one thread without further splitting into smaller threads. This may provide some optimization, as each smallest unit-of-work thread is a much smaller problem than the entire problem, meaning that a task scheduler could have an easier time "fitting in" the threads in the small time interval that they have.
3. Desktop GUI for OS
    - In the desktop GUI for an OS, it could be important that threads may be created and run concurrently. Obviously, I/O, file manipulation, updates/downloads, and windowing should all be separate *processes*, that much is clear to see. Why, however, would we need a multithreading solution here? The main benefits of multithreading: (1) responsiveness, (2) resource sharing, (3) economy, and (4) scalability, show why this is a good use case for multithreading. In the aforementioned file manipulation scenario, we could benefit from having a single process to handle all file manipulation, which is further split into threads. This way, if we need to access 5 separate folders for a number of running applications, each application can talk directly to the file manipulation process, and that file manipulation process may create 5 separate threads to execute the body-of-work for each inquiring application. This makes sense, because permissions, stack space, code, etc. doesn't need to be copied into separate processes, which would create redundancy and incur overhead.

## 4.2
### Using Amdahl’s Law, calculate the speedup gain of an application that  has a 60 percent parallel component for (a) two processing cores and (b) four processing cores.
Recall Amdahl's equation:
```
speedup <= 1 / (S + ((1-S)/N))
``` 
1. (a) If there is a 60% parallel component, then 40%, or .4 (S), of the application must run serially. Additionally, there are 2 processing cores (N). Substituting these values for S and N into the following Amdahl's equation, we will get:
```
speedup <= 1 / (0.4 + ((1-0.4)/2)) ≈ 1.429
``` 
2. (a) If there is a 60% parallel component, then 40%, or .4 (S), of the application must run serially. Additionally, there are 4 processing cores (N). Substituting these values for S and N into the following Amdahl's equation, we will get:
```
speedup <= 1 / (0.4 + ((1-0.4)/4)) ≈ 1.818
``` 

## 4.3
### Does the multithreaded web server in Section 4.1 exhibit task or data parallelism?
That example exhibits primarily task parallelism. Our goal is not to perform operations on the same data, or to separate the data into separate chunks that can be processed concurrently, as would be the case in a data parallelism scenario. Rather, with a web server, we have multiple units of work (*tasks*) which need to be run, and can be separated out into separate chunks of work. For example, displaying a picture on a website can benefit from task parallelism because it is a completely separate task which we sould like to run concurrently with another task or set of tasks like taking keyboard input, management of the UI of the browser, and other tasks.

## 4.4
### What are two differences between user-level threads and kernel-level threads? Under which circumstances is one type better than the other?
User level threads are implemented by the users, and kernel threads are implemented by the operating system. Also, the implementation of user-level threads is typically much simpler than that of kernel threads. Hence, kernel threads are slower to create. User level threads are simpler because they are a sort of abstraction away from the hardware. It is important to note that the actual CPU cores do not execute user-level threads. Instead, user-level threads must be mapped onto "real" kernel threads in order to actually execute. If an applicatino programmer is developing a new Windows application that is a messaging service, they should like to create a UI thread, perhaps a server updating thread, and a business logic thread. These make sense as user-level threads, but the operating system may prefer to organize the threads in a separate way, say by perhaps implementing the business logic and server interfacing thread through a single kernel thread, while granting the user-level UI thread its own kernel thread. In this way, the UI and business logic OR server interface could run not only concurrently, but concurrently AND parallel. However, thee business logic and server interface could run concurrently, but NOT in parallel (at the same time).

## 4.5
### Describe the actions taken by a kernel to context-switch between kernel-level threads
Context switching between kernel threads typically requires saving the value of the registers of the thread being switched out and restoring the values of the registers of the new thread.

## 4.6
### What resources are used when a thread is created? How do they differ from those used when a process is created?
Typically, when a process is created, memory is allocated for the stack space, heap space, data, text, and more. In essence, if a ```fork()``` call is made, everything in the parent process is **copied** to create a new process. However, threads typically have access to all of the global data, heap space, and other shared data. Each thread gets its own stack space, but accesses shared heap memory.

## 4.7
### Assume that an operating system maps user-level threads to the kernel using the one-to-many model and that the mapping is done through LWPs. Furthermore, the system allows developers to create real-time threads for use in real-time systems. Is it necessary to bind a real-time thread to an LWP? Explain.
First, recall that LWPs, or lightweight processes, act as a sort of virtual processor onto which the application can schedule a user thread to run. Each LWP is attached to kernel thread, and it is those kernel threads that the operating system schedules to run on physical CPU cores.
Yes, it should be necessary for the system to bind a real-time thread to an LMP! Because a real-time thread must be run as quickly as possible, we can't put ourselves in a position where a real-time thread needs to wait on other real-time threads to execute before it can execute. This is why the one-to-many model will not work in this instance. If multiple threads are mapped to a single LWP, then the LWP can only execute one of the threads at a time, seeing as how each LWP is attached to exaactly one kernel thread. This is where a two-level model for our system would make sense. Here, all of the user-level threads could be mapped to one LWP in a one-to-many fashion, and all of the real-time threads attach to their own LWP, each of which is attached to its own kernel. That way, each real-time thread is given immediate priority, and doesn't need to wait for other threads to finish.

## 4.8
### Provide two programming examples in which multithreading does *not* provide better performance than a single-threaded solution
Any kind of program which must run sequentially is a good candidate to avoid multithreading. An example of this might be a program that calculates tax returns. This must obviously be done sequentially. Another example is a shell program such as the C-shell (csh) or Bourne-again shell (bash). These programs run quite sequentially, and must closely monitor its own working spaces, watching things like open files, environment variables, the current working directory, etc. 

## 4.9
### Under what circumstances does a multithreaded solution using multiple kernel threads provide better performance than a single-threaded solution on a single-processor system? 
Any situations in which the work that process needs to do can be separated into independent tasks is a good time for multithreading with multiple kernel threads. With multiple kernel threads, multiple things can be run at once, on different processing cores, if their execution does not rely on the results of other threads. Otherwise, the operating system truly could not do multiple things at once. Such a system could certainly emulate doing multiple things at once by rapidly context switching at regular time intervals, but this context switching alone would incur lots of overhead, in addition to the fact that nothing would be running concurrently.

## 4.10
### Which of the following components of program state are shared across threads in a multithreaded process?
### - Register Values
### - Heap memory
### - Global variables
### - Stack memory
In a multithreaded process, Global variables and heap memory are shared across threads while stack memory and register values are unique/private to each thread.

## 4.11
### Can a multithreaded solution using multiple user-level threads achieve better performance on a multiprocessor system than on a  single-processor system? Explain.
The answer is no, and yes. A multithreaded solution with multiple user-level threads **can** achieve better performance on a multiprocessor system than on a single-processor system. **If we assume that we have atleast a one-to-one or many-to-many architecture, wherein each user thread may be mapped to its own kernel thread, or, at the very least, we have access to multiple kernel threads, then we can certainly gain better performance with multiple processing cores**. With a single core, the system could only run one kernel thread at a time. This means we would have **no parallelism with a single core**. We would achieve concurrency, but no parallelism. **However**, if we have mutliple cores, then multiple kernel threads can/will run in parallel (at the same time). Therefore, if our threads could truly run completely independent of each other, and didn't wait for the execution or results of any other thread, then a dual-core system could theoretically, in a perfect universe, run twice as fast as a single-core system.

## 4.12
### In Chapter 3, we discussed Google’s Chrome browser and its practice of opening each new tab in a separate process. Would the same benefits have been achieved if, instead, Chrome had been designed to open each new tab in a separate thread? Explain. 
No, because one of the primary reasons that a new process was craeted for each browser tab was so that if a single webpage crashed, it would not crash the entire browser. If you used a thread for each tab, then the browser and all of its tabs will be inside of a single process, and if a tab were to crash, the entire application would crash along with it. However, Google Chrome is known to be a memory hog, so if there were a way to mitigate this issue without the creation of separate processes for each tab, then spawning new tabs instead of new processes could help the application use less memory.

## 4.13
### Is it possible to have concurrency but not parallelism? Explain.
Yes! Concurrency means that we can run multiple threads or tasks together in the same overlapped time perioud, but it does not mean that we are running them at the same instance in time. If I start two tasks at the same time, and I intermittently switch between the two tasks in order to finish them more efficiently, then I have achieved concurrency. Parallelism, on the other hand, means that I can run multiple tasks at the same instance in time. Therefore, if I have parallelism, then I must also have concurrency, but I can have concurrency without parallelism.

## 4.14
### Using Amdahl's Law, calculate the speedup gain for the following applications:
### 1. 40 percent parallel with (a) 8 processing cores and (b) 16 processing cores
### 2. 67 percent parallel with (a) 2 processing cores and (b) 4 processing cores
### 3. 90 percent parallel with (a) 4 processing cores and (b) 8 processing cores
Recall Amdahl's Equation:
```
speedup <= 1 / (S + ((1-S)/N))
```
1. 
    - (a) 
    ``` 
    speedup <= 1 / (.6 + ((1-.6)/8)) ≈ 1.538
    ``` 
    - (b) 
    ``` 
    speedup <= 1 / (.6 + ((1-.6)/16)) = 1.6
    ``` 
2. 
    - (a) 
    ``` 
    speedup <= 1 / (.33 + ((1-.33)/2)) ≈ 1.504
    ``` 
    - (b) 
    ``` 
    speedup <= 1 / (.33 + ((1-.33)/4)) = 2.010
    ``` 
3. 
    - (a) 
    ``` 
    speedup <= 1 / (.1 + ((1-.1)/4)) ≈ 3.077
    ``` 
    - (b) 
    ``` 
    speedup <= 1 / (.1 + ((1-.1)/8)) = 4.706
    ``` 

## 4.15
### Determine if the following problems exhibit task or data parallelism
### 1. Using a separate thread to generate a thumbnail for each photo in a collection
### 2. Transposing a matrix in parallel
### 3. A networked application where one thread reads from the ntwork kand another writes to the network
### 4. The fork-join array summation application described in Section 4.5.2
### 5. The Grand Central Dispatch System
1. Data parallelism - We want to perform the same sort of task/computation on a large set of data, so our goal in making a parallel solution is to do this faster by simply splitting up the data and performing computation on different chunks of the data at the same time.
2. Data parallelism - We have one task to do: transposing a matrix. This task can be made faster by simply transposing smaller cha=unks of the matrix (which is our data)
