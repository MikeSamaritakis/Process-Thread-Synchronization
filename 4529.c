// /* Michail Samaritakis csd4529
//  * Assignment 2 HY-345 2023-24
// */

// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <semaphore.h>
// #include <unistd.h>

// #define NUM_STUDENTS
// #define MAX_STUDY_TIME 15

// sem_t study_room;
// sem_t waiting_room;
// pthread_t students[NUM_STUDENTS];

// void *student_thread(void *arg) {
//     int id = *(int *)arg;
//     int study_time = (rand() % (MAX_STUDY_TIME - 5 + 1)) + 5; // Random study time between 5 to 15 seconds

//     printf("Student %d wants to enter the study room.\n", id);

//     sem_wait(&waiting_room);

//     printf("Student %d entered the waiting room.\n", id);
    
//     sem_wait(&study_room);
//     sem_post(&waiting_room);

//     printf("Student %d entered the study room.\n", id);
    
//     sleep(study_time);

//     printf("Student %d exited the study room after studying for %d seconds.\n", id, study_time);
//     sem_post(&study_room);

//     return NULL;
// }

// int main(int argc, char * argv[]) {
    
//     /* Checking for invalid values of students. */
//     if (atoi(argv[1]) < 20 || atoi(argv[1]) > 40){
//         perror("Invalid number of students.\nPlease enter a number between 20 and 40.\n");
//         return -1;
//     }

//     sem_init(&study_room, 0, 8);
//     sem_init(&waiting_room, 0, NUM_STUDENTS);

//     int student_ids[NUM_STUDENTS];

//     for (int i = 0; i < NUM_STUDENTS; i++) {
//         student_ids[i] = i + 1;
//         pthread_create(&students[i], NULL, student_thread, &student_ids[i]);
//     }

//     for (int i = 0; i < NUM_STUDENTS; i++) {
//         pthread_join(students[i], NULL);
//     }

//     sem_destroy(&study_room);
//     sem_destroy(&waiting_room);

//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define MAX_STUDY_TIME 15

sem_t study_room;
sem_t waiting_room;
pthread_t *students;

int num_students;

int waiting_students[40]; // Array to track students in the waiting room
int studying_students[8]; // Array to track students in the study room

void printRoad() {
    //system("clear"); // Clear the console for updated display
    printf("Study Room:\n");
    for (int i = 0; i < 8; i++) {
        printf("| ");
        if (studying_students[i] != -1) {
            printf("S%02d ", studying_students[i] + 1);
        } else {
            printf("    ");
        }
    }
    printf("|\n");
    printf("Waiting Room:\n");
    for (int i = 0; i < num_students; i++) {
        printf("| ");
        if (waiting_students[i] != -1) {
            printf("S%02d ", waiting_students[i] + 1);
        } else {
            printf("    ");
        }
    }
    printf("|\n");
    fflush(stdout);
}

void removeStudentFromArray(int student_id, int array[], int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == student_id) {
            array[i] = -1; // Mark the slot as empty
            break;
        }
    }
}

void *student_thread(void *arg) {
    int id = *(int *)arg;
    int study_time = (rand() % (MAX_STUDY_TIME - 5 + 1)) + 5; // Random study time between 5 to 15 seconds

    printf("Student %d wants to enter the study room.\n", id);

    sem_wait(&waiting_room);

    printf("Student %d entered the waiting room.\n", id);
    waiting_students[id - 1] = id;
    
    sem_wait(&study_room);
    sem_post(&waiting_room);

    printf("Student %d entered the study room.\n", id);
    removeStudentFromArray(id, waiting_students, num_students);
    studying_students[id - 1] = id;

    //printRoad(); // Print the road after entering the study room

    sleep(study_time);

    printf("Student %d exited the study room after studying for %d seconds.\n", id, study_time);
    sem_post(&study_room);
    removeStudentFromArray(id, studying_students, 8);

    //printRoad(); // Print the road after exiting the study room

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Usage: ./your_program <num_students>\n");
        return -1;
    }

    num_students = atoi(argv[1]);

    if (num_students < 20 || num_students > 40) {
        perror("Invalid number of students. Please enter a number between 20 and 40.\n");
        return -1;
    }

    students = (pthread_t *)malloc(num_students * sizeof(pthread_t));

    sem_init(&study_room, 0, 8);
    sem_init(&waiting_room, 0, num_students);

    int student_ids[num_students];

    for (int i = 0; i < num_students; i++) {
        student_ids[i] = i + 1;
        waiting_students[i] = -1; // Initialize waiting room slots
        pthread_create(&students[i], NULL, student_thread, &student_ids[i]);
    }

    for (int i = 0; i < num_students; i++) {
        pthread_join(students[i], NULL);
    }

    sem_destroy(&study_room);
    sem_destroy(&waiting_room);
    free(students);

    return 0;
}
