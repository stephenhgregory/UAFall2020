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
speedup <= 1 / (1 + ((1-S)/N))
``` 
1. (a) If there is a 60% parallel component, then 40%, or .4 (S), of the application must run serially. Additionally, there are 2 processing cores (N). Substituting these values for S and N into the following Amdahl's equation, we will get:
```
speedup <= 1 / (1 + ((1-0.4)/2)) ≈ 0.769
``` 
2. (a) If there is a 60% parallel component, then 40%, or .4 (S), of the application must run serially. Additionally, there are 4 processing cores (N). Substituting these values for S and N into the following Amdahl's equation, we will get:
```
speedup <= 1 / (1 + ((1-0.4)/4)) ≈ 0.870
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