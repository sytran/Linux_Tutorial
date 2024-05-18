// =============================================================================
//
// -----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h> // for uintptr_t

#define NUM_THREADS 3

// Global semaphores
sem_t sem_A, sem_B, sem_C;

// Function prototypes
void *thread_function(void *arg);
void timer_handler(int signum);

int main() {
    pthread_t threads[NUM_THREADS];

    // Initialize semaphores
    sem_init(&sem_A, 0, 0);
    sem_init(&sem_B, 0, 0);
    sem_init(&sem_C, 0, 0);

    // Create threads
    pthread_create(&threads[0], NULL, thread_function, (void *)(uintptr_t)&sem_A);
    pthread_create(&threads[1], NULL, thread_function, (void *)(uintptr_t)&sem_B);
    pthread_create(&threads[2], NULL, thread_function, (void *)(uintptr_t)&sem_C);

    // Set up signal handler for alarm
    signal(SIGALRM, timer_handler);

    // Set timers using alarm
    alarm(1); // Timer A
    alarm(2); // Timer B
    alarm(5); // Timer C

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&sem_A);
    sem_destroy(&sem_B);
    sem_destroy(&sem_C);

    return 0;
}

void *thread_function(void *arg) {
    sem_t *sem = (sem_t *)(uintptr_t)arg;
    printf("Start thread %c \n", (char)(((intptr_t)arg - (intptr_t)&sem_A) + 'A'));

    while (1) {
        sem_wait(sem);
        printf("Thread %c is unblocked.\n", (char)(((intptr_t)arg - (intptr_t)&sem_A) + 'A'));
        // Do thread-specific operations here
        sleep(1); // Simulate work
    }

    pthread_exit(NULL);
}

void timer_handler(int signum) {
    sem_t *sem;
    switch (signum) {
        case SIGALRM:
            sem = &sem_A;
            break;
        case SIGALRM + 1:
            sem = &sem_B;
            break;
        case SIGALRM + 2:
            sem = &sem_C;
            break;
        default:
            return;
    }

    sem_post(sem);
}
