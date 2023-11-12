// /* Michail Samaritakis csd4529
//  * Assignment 2 HY-345 2023-24
// */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define MAX_STUDY_TIME 15
#define MAX_STUDENTS 40

sem_t study_room;
sem_t waiting_room;
pthread_t *students;

int num_students;

int waiting_students[40]; // Array to track students in the waiting room.
int studying_students[8]; // Array to track students in the study room.

void removeStudentFromArray(int am, int array[], int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == am) {
            array[i] = -1; // Mark the slot as empty by having -1 as the value.
            break;
        }
    }
}

void printRoooms() {

    sem_wait(&study_room);
    sem_wait(&waiting_room);
    //system("clear"); // Clear the console for updated display
    printf("Study Room:");
    for (int i = 0; i < 8; i++) {
        if (studying_students[i] == -1){
            //printf(" ");
        }else{
            printf("| ");
        }
        if (studying_students[i] != -1) {
            printf("%02d ", studying_students[i] + 1);
        } else {
            //printf("  ");
        }
    }
    printf("|\n");

    printf("Waiting Room:");
    for (int i = 0; i < num_students; i++) {
        if (waiting_students[i] == -1){
            //printf(" ");
        }else{
            printf("| ");
        }
        if (waiting_students[i] != -1) {
            printf("%02d ", waiting_students[i] + 1);
        } else {
            //printf("  ");
        }
    }
    printf("|\n");
    printf("\n");
    fflush(stdout);

    sem_post(&study_room);
    sem_post(&waiting_room);

    return;
}

void *student_thread(void *arg) {
    int id = *(int *)arg;
    int study_time = (rand() % (MAX_STUDY_TIME - 5 + 1)) + 5; // Random study time between 5 to 15 seconds.
    int name = rand() % 9000 + 1000;

    //printf("Student %d wants to enter the study room.\n", id);

    sem_wait(&waiting_room);

    ///printf("Student %d entered the waiting room.\n", id);
    waiting_students[id - 1] = name;
    
    sem_wait(&study_room);
    
    sem_post(&waiting_room);

    //printf("Student %d entered the study room.\n", id);
    removeStudentFromArray(name, waiting_students, num_students);
    studying_students[id - 1] = name;

    printRoooms(); // Print the road after entering the study room.

    sleep(study_time);

    printf("Student %d exited the study room after studying for %d seconds.\n", name+1, study_time);
    sem_post(&study_room);
    removeStudentFromArray(name, studying_students, 8);

    printRoooms(); // Print the road after exiting the study room.

    return NULL;
}



int main(/*int argc, char *argv[]*/){

    // /* Arguement check for correct use. */
    // if (argc != 2) {
    //     perror("Try typing: './4529 <number>' where 20 <= number <= 40 .\n");
    //     return -1;
    // }

    // num_students = atoi(argv[1]); /* Arguement given by user for the amount of students. */

    // /* Arguement check for correct input number. */
    // if (num_students < 20 || num_students > 40) {
    //     perror("Invalid number of students, please enter a number between 20 and 40.\n");
    //     return -1;
    // }

    for(int i = 0; i < 40; i++){
        waiting_students[i] = -1;
        studying_students[i] = -1;
    }

    /* Arguement given by user for the amount of students. */
    //num_students = atoi(argv[1]); 
    int temp = -1;
    printf("Enter a number between 20 and 40 for the amount of students:");
    scanf("%d", &temp);
    while (temp < 20 || temp > 40){
        perror("Invalid number of students, please enter a number between 20 and 40.\n");
        printf("Enter a number between 20 and 40 for the amount of students:");
        scanf("%d", &temp);
    }
    
    num_students = temp;

    /* Allocate space for the amount of threads specified by user through input. */
    students = (pthread_t *)malloc(num_students * sizeof(pthread_t)); 

    /* The sem_init function is used to initialize the study_room and waiting_room semaphores.
    * The study_room semaphore is initialized with a value of 8, indicating that there are 8 
    * slots available in the study room. The waiting_room semaphore is initialized with a 
    * value of num_students, indicating that all students can wait in the waiting room. */
    sem_init(&study_room, 0, 8);
    sem_init(&waiting_room, 0, num_students);

    /* An array student_ids is created to hold the IDs of the students. 
    * The IDs are simply integers from 1 to num_students. */
    int student_ids[num_students];

    /* A for loop is used to initialize the waiting_students array and create the student
    * threads. The pthread_create function is used to create a new thread that starts
    * execution in the student_thread function. The address of the student's ID is 
    * passed as the argument to the student_thread function. */
    for (int i = 0; i < num_students; i++) {
        student_ids[i] = i + 1;
        waiting_students[i] = -1; // Initialize waiting room slots
        pthread_create(&students[i], NULL, student_thread, &student_ids[i]);
    }

    /* Another for loop is used to wait for all student threads to finish execution.
    * The pthread_join function blocks the calling thread 
    * (in this case, the main thread) until the specified thread (the student thread) 
    * finishes execution. */
    for (int i = 0; i < num_students; i++) {
        pthread_join(students[i], NULL);
    }

    /* After all threads have finished execution, the sem_destroy 
    * function is used to destroy the study_room and waiting_room semaphores. */
    sem_destroy(&study_room);
    sem_destroy(&waiting_room);
    
    /* The free function is used to deallocate the memory that was previously
    * allocated for the students array. This is important to prevent memory leaks. */
    free(students);

    return 0;
}AAAAAAAAAAAAAA