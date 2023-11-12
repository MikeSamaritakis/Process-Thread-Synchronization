/* FRESKOU ASPASIA
csd4762
ASSIGNMENT 2*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>


typedef struct {
    int ID;
    int isInReadingRoom;
} Student;

pthread_t student_thread[40];
Student* study_room[8];
Student* waiting_room[40];
int number;
int students_in_waiting_room = 0;
pthread_mutex_t roomMutex;



void printRooms() {
    printf("Study room: |");
    for (int i = 0; i < 8; i++) {
        if (study_room[i] != NULL) {
            printf(" %d |", study_room[i]->ID);
        } else {
            printf(" |");
        }
     }
    printf("\n");
    printf("Waiting room: |");
    for (int i = 0; i < students_in_waiting_room; i++) {
        printf(" %d |", waiting_room[i]->ID);
    }
    printf("\n");
}

void* thread_implementation(void* arg) {
    Student* student = (Student*)arg;
    int id = student->ID;

    int timeStudy = 5 + (rand() % 11);  /* random time for studying (5-15 seconds) */

    pthread_mutex_lock(&roomMutex);
    printf("Student %d was born.\n", id);

    int emptySpot = -1; /* Search for an empty position in the study room to add student. */
    for (int i = 0; i < 8; i++) {
        if (study_room[i] == NULL) {
            printf("Student %d entered the study room.\n", id);
            study_room[i] = student;
            emptySpot = i;
            break;
        }
    }

    if (emptySpot == -1) {
        if (students_in_waiting_room < 40) {  // Ensure waiting room isn't full
            waiting_room[students_in_waiting_room] = student;
            students_in_waiting_room++;
            printf("Student %d is in the waiting room.\n", id);
        } else {
            printf("Student %d is leaving because the study room and waiting room are full.\n", id);
            free(student);  // Free the student if they leave
        }
    }

        printRooms();
        pthread_mutex_unlock(&roomMutex);
       
        sleep(timeStudy);

        pthread_mutex_lock(&roomMutex);
        
        int isRoomEmpty = 0;
        int counter = 0;
        if (emptySpot != -1) {
            study_room[emptySpot] = NULL;
            printf("Student %d is leaving after %d seconds of studying.\n", id, timeStudy);
            printRooms();
            free(student);  // Free the student if they leave
	        student = NULL;

            for (int i = 0; i < 8; i++) {
                if (study_room[i] == NULL) {
                    counter++;
                }
            }
            if (counter == 8) isRoomEmpty = 1;
        }

    // Check if the room is empty, and if so, allow the next group of students from the waiting room to enter
    while (isRoomEmpty && students_in_waiting_room > 0) {
        printf("Study room is empty, allowing the next group of students to enter.\n");
        int enteredCount = 0;  // the number of students that entered
        int enteredStudents[8];    //the ID of students that entered
        for (int i = 0; i < 8; i++) {
            enteredStudents[i] = -1;  // Initialize 
        }

        for (int i = 0; i < 8; i++) {
            if (students_in_waiting_room > 0) {
                for (int j = 0; j < 8; j++) {     // Find an empty position in study room
                    if (study_room[j] == NULL) {
                        study_room[j] = waiting_room[0];
                        enteredStudents[enteredCount] = waiting_room[0]->ID;
                        printf("Student %d entered the study room.\n", waiting_room[0]->ID);
                        for (int k = 0; k < students_in_waiting_room - 1; k++) {    //put the students from waiting room to study room
                            waiting_room[k] = waiting_room[k + 1];
                        }
                        waiting_room[students_in_waiting_room - 1] = NULL;
                        students_in_waiting_room--;
                        enteredCount++;
                        break;
                    }
                }
                printRooms();
            }
        }

        
        int TimeofStudy[8];                //random time of studying in study room
        int printTimeofStudy[8];
        for (int i = 0; i < 8; i++) {
            if (TimeofStudy[i] != -1) {
            TimeofStudy[i] = 5 + (rand() % 11);
            printTimeofStudy[i]=TimeofStudy[i];
            }
        }

        while (enteredCount > 0) {                //until all students studied
            for (int i = 0; i < 8; i++) {
                if (enteredStudents[i] != -1) {
                    if (TimeofStudy[i] > 0) {
                    TimeofStudy[i]--;
                    } else {
                        printf("Student %d is leaving after %d seconds of studying.\n", enteredStudents[i], printTimeofStudy[i]);
                        for (int j = 0; j < 8; j++) {
                            if (study_room[j] != NULL && study_room[j]->ID == enteredStudents[i]) {
                                study_room[j] = NULL;
                                break;
                            }
                        }
                        printRooms();
                        free(student);  // Free the student if they leave
                        student = NULL;
                        enteredStudents[i] = -1; // Mark as left
                        enteredCount--;
                    }
                }
            }
        }

        // Check if the study room is empty
        isRoomEmpty = 1;
        for (int i = 0; i < 8; i++) {
            if (study_room[i] != NULL) {
                isRoomEmpty = 0;
                break;
            }
        }
    }

    pthread_mutex_unlock(&roomMutex);    
    
    return;
}



void initializeResources() {
    pthread_mutex_init(&roomMutex, NULL);
}

void destroyResources() {
    pthread_mutex_destroy(&roomMutex);
}

int main() {
    srand(time(NULL));
    for (int i = 0; i < 8; i++) {
        study_room[i] = NULL;
    }

    while (1) {
        printf("Please enter the number of students: ");
        if (scanf("%d", &number) != 1 || number > 40 || number < 20) {
            printf("Invalid number of students. Please enter a number between 20 and 40.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        } else {
            break; // Exit the loop when a valid number is provided
        }
    }

    initializeResources();

    for (int j = 0; j < number; j++) {
        Student* student = (Student*)malloc(sizeof(Student));
        student->ID = j;
        student->isInReadingRoom = 0;
        pthread_create(&student_thread[j], NULL, thread_implementation, student);
    }


    for (int j = 0; j < number; j++) {
        pthread_join(student_thread[j], NULL);
    }

    destroyResources();


    return 0;
}
