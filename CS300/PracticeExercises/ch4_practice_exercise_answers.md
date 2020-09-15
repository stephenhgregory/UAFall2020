# Chapter 4 Practice Exercise Answers

## 4.1
### Provide three programming examples in which multithreading provides better performance than a single-threaded solution
1. Web Browser
    - In a web browser, it makes a lot of sense for us to have multiple threads. Within one single tab, on one webpage, we may have separate threads for user input, HTTP requests, and webpage rendering. That way, we can be posting an HTTP POST request to a server while a video is still playing on the screen. Ruthermore, we could also type in a search bar while the return code for the HTTP POST request is still not received, and the video is still playing. Without multithreading, the the video would have to stop playing while our HTTP POST request is being sent and waited on, and clicking and typing on the search bar wouldn't work. The whole thing would appear to "freeze".
2. Divide and Conquer algorithms
    - In a divide-and-conquer algorithm such as merge-sort, each recursively created subproblem can be created as a separate thread. This is a perfect use-case for multithreading, because each subproblem does not need to wait for the other subproblems to compute. In this case, each thread can run concurrently, as each is an independent unit of work. Potentially, the end of the recursive task-creation could be triggered at a small constant size. In this way,  