/* Michail Samaritakis csd4529
 * Assignment 2 HY-345 2023-24
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_STUDENTS 20
#define MAX_STUDY_TIME 15

sem_t study_room;
sem_t waiting_room;
pthread_t students[NUM_STUDENTS];

void *student_thread(void *arg) {
    int id = *(int *)arg;
    int study_time = (rand() % (MAX_STUDY_TIME - 5 + 1)) + 5; // Random study time between 5 to 15 seconds

    printf("Student %d wants to enter the study room.\n", id);

    sem_wait(&waiting_room);

    printf("Student %d entered the waiting room.\n", id);
    
    sem_wait(&study_room);
    sem_post(&waiting_room);

    printf("Student %d entered the study room.\n", id);
    
    sleep(study_time);

    printf("Student %d exited the study room after studying for %d seconds.\n", id, study_time);
    sem_post(&study_room);

    return NULL;
}

int main(int argc, char * argv[]) {
    
    /* Checking for invalid values of students. */
    if (atoi(argv[1]) < 20 || atoi(argv[1]) > 40){
        perror("Invalid number of students.\nPlease enter a number between 20 and 40.\n");
        return -1;
    }

    sem_init(&study_room, 0, 8);
    sem_init(&waiting_room, 0, NUM_STUDENTS);

    int student_ids[NUM_STUDENTS];

    for (int i = 0; i < NUM_STUDENTS; i++) {
        student_ids[i] = i + 1;
        pthread_create(&students[i], NULL, student_thread, &student_ids[i]);
    }

    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(students[i], NULL);
    }

    sem_destroy(&study_room);
    sem_destroy(&waiting_room);

    return 0;
}
