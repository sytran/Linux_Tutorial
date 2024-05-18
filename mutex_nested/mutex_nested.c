#include <stdio.h>      // For printf and fprintf
#include <stdlib.h>     // For exit and EXIT_FAILURE
#include <pthread.h>    // For pthread functions
#include <unistd.h>     // For sleep function

// Define the mutexes
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

unsigned long sharedVariable = 0; // Shared variable between threads

// ------------------------------------------------------------
// Function executed by Thread 1
void* thread1_function(void* arg) {
    for (int i = 0; i < 5; i++) {
        // Lock the first mutex
        printf("Thread 1: trying to lock mutex1\n");
        pthread_mutex_lock(&mutex1);
        printf("Thread 1: locked mutex1\n");

        // Simulate some work
        usleep(10);

        // Try to lock the second mutex
        printf("Thread 1: trying to lock mutex2\n");
        pthread_mutex_lock(&mutex2);
        printf("Thread 1: locked mutex2\n");

        for (unsigned long j = 0; j < 10000; j++) {
        	for (unsigned long k = 0; k < 10000; k++) {
        		// simulate long processing
        	}
    		sharedVariable++; // Increment shared variable
        }
        printf("Thread 1: Shared Count = %lu\n", sharedVariable); // Print thread ID and count value

        // Unlock the mutexes
        printf("Thread 1: unlocked mutex2\n");
        pthread_mutex_unlock(&mutex2);
        printf("Thread 1: unlocked mutex1\n");
        pthread_mutex_unlock(&mutex1);

    }
    return NULL;
}

// ------------------------------------------------------------
// Function executed by Thread 2
void* thread2_function(void* arg) {
    for (int i = 0; i < 5; i++) {
        // Lock the second mutex
        printf("Thread 2: trying to lock mutex1\n");
    	pthread_mutex_lock(&mutex1);
        printf("Thread 2: locked mutex1\n");

        // Simulate some work
        usleep(10);

        // Try to lock the first mutex
        printf("Thread 2: trying to lock mutex2\n");
        pthread_mutex_lock(&mutex2);
        printf("Thread 2: locked mutex2\n");

        for (unsigned long j = 0; j < 10000; j++) {
        	for (unsigned long k = 0; k < 10000; k++) {
        		// simulate long processing
        	}
    		sharedVariable++; // Increment shared variable
        }
        printf("Thread 2: Shared Count = %lu\n", sharedVariable); // Print thread ID and count value

        // Unlock the mutexes
        printf("Thread 2: unlocked mutex2\n");
        pthread_mutex_unlock(&mutex2);
        printf("Thread 2: unlocked mutex1\n");
        pthread_mutex_unlock(&mutex1);

    }
    return NULL;
}

// ------------------------------------------------------------
// 
int main() {
    pthread_t thread1, thread2;

    // Initialize the mutexes
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    // Create the threads
    pthread_create(&thread1, NULL, thread1_function, NULL);
    pthread_create(&thread2, NULL, thread2_function, NULL);

    // Wait for the threads to complete
    for (int i = 0; i < 5; i++) {
        int ret1 = pthread_join(thread1, NULL);
        int ret2 = pthread_join(thread2, NULL);

        if (ret1 != 0 || ret2 != 0) {
            fprintf(stderr, "Potential Deadlock detected after %d iterations\n", i+1);
            exit(EXIT_FAILURE);
        }
    }

    // Destroy the mutexes
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}
