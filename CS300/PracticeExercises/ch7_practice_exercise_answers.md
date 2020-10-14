# Chapter 7 Practice Exercise Answers



## 7.14
### What are the names of the two processes associated with the bounded buffer problem?
- The **producer** and **consumer** processes

## 7.15
### How many writers may concurrently share the database with the readers-writers problem?
- In the readers-writers problem, only one writer can access the database at a given time. On the other hand, an unlimited number of readers can access the database at a given time.

## 7.16
### What is the problem if all of the philosophers simultaneously pick up their left fork?
- First of all, this problem description is silly and unintuitive when we describe it as philisophers waiting for available chopsticks with which to eat their spaghetti. We will describe the problom as philisophers waiting for available chopsticks with which to eat their noodles. Each philosopher needs both chopsticks in order to eat their meal. If they all pick up their left chopstick simultaneously, then all of the philosophers will then attempt to pick up their right chopstick, and will block indefinitely while doing so. Here's the catch: all of the chopsticks are now already taken, so the philosophers will be waiting for their right chopstick to become available, but every philosopher is blocking, so the chopstick will never become available. This is called a "circular wait" and results in this condition that we call **deadlock**.

