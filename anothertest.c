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

int waiting_students[40]; // Array to track students in the waiting room
int studying_students[8]; // Array to track students in the study room

void removeStudentFromArray(int student_id, int array[], int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == student_id) {
            array[i] = -1; // Mark the slot as empty
            break;
        }
    }
}

int main(int argc, char *argv[]){

    /* Arguement check for correct use. */
    if (argc != 2) {
        perror("Try typing: './4529 <number>' where 20 <= number <= 40 .\n");
        return -1;
    }

    num_students = atoi(argv[1]); /* Arguement given by user for the amount of students. */

    /* Arguement check for correct input number. */
    if (num_students < 20 || num_students > 40) {
        perror("Invalid number of students, please enter a number between 20 and 40.\n");
        return -1;
    }

    for(int i = 0; i < 40; i++){
        waiting_students[i] = -1;
        studying_students[i] = -1;
    }

    num_students = atoi(argv[1]); /* Arguement given by user for the amount of students. */

    /* Allocate space for the amount of threads specified by user through input. */
    students = (pthread_t *)malloc(num_students * sizeof(pthread_t)); 

    

    return 0;
}