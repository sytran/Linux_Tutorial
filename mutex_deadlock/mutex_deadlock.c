#include <stdio.h>      // For printf and fprintf
#include <stdlib.h>     // For exit and EXIT_FAILURE
#include <pthread.h>    // For pthread functions
#include <unistd.h>     // For sleep function
#include <stdlib.h>     // For rand, srand, and RAND_MAX

// Define the mutexes
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

unsigned long sharedVariable = 0; // Shared variable between threads

// ------------------------------------------------------------
// Generates a random number between 0 and 10.
// return int - A random number between 0 and 10.
int generate_random_number() {
    return rand() % 11; // Generates a random number between 0 and 10 inclusive
}

// ------------------------------------------------------------
// Function executed by Thread 1
void* thread1_function(void* arg) {
    for (int i = 0; i < 5; i++) {
        // Lock the first mutex
        printf("Thread 1: trying to lock mutex1\n");
        pthread_mutex_lock(&mutex1);
        printf("Thread 1: locked mutex1\n");

        // Simulate some work
        sleep(1);

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
        printf("Thread 2: unlocked mutex2\n");
        pthread_mutex_unlock(&mutex2);
        printf("Thread 2: unlocked mutex1\n");
        pthread_mutex_unlock(&mutex1);
    }
    return NULL;
}

// ------------------------------------------------------------
// Function executed by Thread 2
void* thread2_function(void* arg) {

    // Seed the random number generator with the current time
    srand(time(NULL));

    // Generate and print a random number between 0 and 10
    int random_number = generate_random_number();
    printf("Random number between 0 and 10: %d\n", random_number);

    for (int i = 0; i < 5; i++) {
        // Lock the second mutex
        printf("Thread 2: trying to lock mutex2\n");
        pthread_mutex_lock(&mutex2);
        printf("Thread 2: locked mutex2\n");

        // Simulate some work
        usleep(random_number);

        // Try to lock the first mutex
        printf("Thread 2: trying to lock mutex1\n");
        pthread_mutex_lock(&mutex1);
        printf("Thread 2: locked mutex1\n");

        for (unsigned long j = 0; j < 10000; j++) {
        	for (unsigned long k = 0; k < 10000; k++) {
        		// simulate long processing
        	}
    		sharedVariable++; // Increment shared variable
        }
        printf("Thread 2: Shared Count = %lu\n", sharedVariable); // Print thread ID and count value

        // Unlock the mutexes

        printf("Thread 2: unlocked mutex1\n");
        pthread_mutex_unlock(&mutex1);
        printf("Thread 2: unlocked mutex2\n");
        pthread_mutex_unlock(&mutex2);
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
            fprintf(stderr, "Deadlock detected after %d iterations\n", i+1);
            exit(EXIT_FAILURE);
        }
    }

    // Destroy the mutexes
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}
