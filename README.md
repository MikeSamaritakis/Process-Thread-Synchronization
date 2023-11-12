
Michail Samaritakis csd4529
Assignment 2 HY-345 2023-24
Process-Thread-Synchronization

This C program simulates a study room scenario using threads and semaphores. It creates a number of student threads, each representing a student trying to access a study room with a limited number of slots. The students wait in a waiting room (also with limited slots) before they can enter the study room. The program keeps track of which students are in the waiting room and the study room using arrays. It also includes a function to remove a student from these arrays. The state of the rooms is printed to the console, showing which slots are occupied and which are empty.

I prefered the use of semaphores rather than mutexes for locking and unlocking purposes.

