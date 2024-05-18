#include <stdio.h>      // Standard input-output library for printing
#include <pthread.h>    // POSIX threads library for multi-threading support
#include <unistd.h>     // POSIX standard library for system calls (used for sleep function)
#include <stdlib.h>     // Standard library for exit function

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

// ------------------------------------------------------------
// Function executed by Thread 1
void* thread1_function(void* arg) {
    // Lock the first mutex
    pthread_mutex_lock(&mutex1);
    printf("Thread 1: locked mutex1\n");

    // Simulate some work
    sleep(1);

    // Try to lock the second mutex
    printf("Thread 1: trying to lock mutex2\n");
    pthread_mutex_lock(&mutex2);
    printf("Thread 1: locked mutex2\n");

    // Unlock the mutexes
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);

    return NULL;
}

// ------------------------------------------------------------
// Function executed by Thread 2
void* thread2_function(void* arg) {
    // Lock the second mutex
    pthread_mutex_lock(&mutex2);
    printf("Thread 2: locked mutex2\n");

    // Simulate some work
    sleep(1);

    // Try to lock the first mutex
    printf("Thread 2: trying to lock mutex1\n");
    pthread_mutex_lock(&mutex1);
    printf("Thread 2: locked mutex1\n");

    // Unlock the mutexes
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);

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
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Destroy the mutexes
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}
