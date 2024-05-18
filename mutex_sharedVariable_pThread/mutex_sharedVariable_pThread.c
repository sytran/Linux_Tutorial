#include <stdio.h>      // Standard input-output library for printing
#include <pthread.h>    // POSIX threads library for multi-threading support
#include <unistd.h>     // POSIX standard library for system calls (used for sleep function)
#include <stdlib.h>     // Standard library for exit function

#define NUM_THREADS 2   // Number of threads

unsigned long sharedVariable = 0; // Shared variable between threads
pthread_mutex_t mutex;  // Mutex to control access to shared variable

// ------------------------------------------------------------
// Function executed by Thread A
void *threadAFunction(void *arg) {
	int *maxCount = (int *)arg;
    for (int i = 0; i < *maxCount; i++) { // Increment the shared variable 5 times
        pthread_mutex_lock(&mutex); // Lock the mutex before accessing shared variable
        for (unsigned long j = 0; j < 10000; j++) {
        	for (unsigned long k = 0; k < 10000; k++) {
        		// simulate long processing
        	}
    		sharedVariable++; // Increment shared variable
        }
        printf("Thread A: Shared Count = %lu\n", sharedVariable); // Print thread ID and count value
        pthread_mutex_unlock(&mutex); // Unlock the mutex after accessing shared variable
        usleep(10); // Sleep for 100 micro seconds (for demonstration purposes)
    }
    pthread_exit(NULL); // Exit the thread
}

// ------------------------------------------------------------
// Function executed by Thread B
void *threadBFunction(void *arg) {
	int *maxCount = (int *)arg;

    for (int i = 0; i < *maxCount; i++) { // Increment the shared variable 5 times
        pthread_mutex_lock(&mutex); // Lock the mutex before accessing shared variable
        for (unsigned long j = 0; j < 10000; j++) {
        	for (unsigned long k = 0; k < 10000; k++) {
        		// simulate long processing
        	}
    		sharedVariable++; // Increment shared variable
        }
        printf("Thread B: Shared Count = %lu\n", sharedVariable); // Print thread ID and count value
        pthread_mutex_unlock(&mutex); // Unlock the mutex after accessing shared variable
        usleep(10); // Sleep for 100 micro seconds (for demonstration purposes)
    }
    pthread_exit(NULL); // Exit the thread
}

// ------------------------------------------------------------
//
int main() {
    pthread_t threadA, threadB; // Thread variables

    pthread_mutex_init(&mutex, NULL); // Initialize the mutex
    int loopCount = 5;

    // Create Thread A
    if (pthread_create(&threadA, NULL, threadAFunction, &loopCount) != 0) {
        perror("Error creating Thread A");
        exit(EXIT_FAILURE);
    }

    // Create Thread B
    if (pthread_create(&threadB, NULL, threadBFunction, &loopCount) != 0) {
        perror("Error creating Thread B");
        exit(EXIT_FAILURE);
    }

    // Wait for Thread A to finish
    if (pthread_join(threadA, NULL) != 0) {
        perror("Error joining Thread A");
        exit(EXIT_FAILURE);
    }

    // Wait for Thread B to finish
    if (pthread_join(threadB, NULL) != 0) {
        perror("Error joining Thread B");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_destroy(&mutex); // Destroy the mutex

    return 0;
}
