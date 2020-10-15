# Chapter 8 Review Answers

## 8.1
### True or False? The system model for deadlocks first requires a process request a resource, then use the resource, and finally release the resource.
True.

## 8.2
### What are the four necessary conditions for characterizing deadlock?
1. **Mutual Exclusion**: At least one resource must be held in a non-shareable mode
2. **Hold and Wait**: A thread must be holding at least one resource and waiting to acquire additional resources that are currently being held by other threads 
3. **No preemption**: Resources cannot be preempted. A resource can only be released voluntarily by the thread holding it, after that thread has completed its task.
4. **Circular wait**: A set {T0, T1, ... , Tn} of waiting threads must exist such that T0 is waiting for a resource held by T1, T1 is waiting for a resource held by T2,..., Tn−1 is waiting for a resource held by Tn, and Tn is waiting for a resource held by T0. 

## 8.3
### Describe one strategy for dealing with deadlocks.
