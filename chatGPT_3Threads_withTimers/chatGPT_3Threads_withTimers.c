// =============================================================================
//
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define NUM_THREADS 3

// Global semaphores
sem_t semaphores[NUM_THREADS];

// Function prototypes
void *thread_function(void *arg);
void timer_handler(int signum, siginfo_t *si, void *data);
void create_timer(timer_t *timer, int timer_id);

int main() {
    pthread_t threads[NUM_THREADS];

    // Initialize semaphores
    for (int i = 0; i < NUM_THREADS; ++i) {
        sem_init(&semaphores[i], 0, 0);
    }

    // Create threads with different priorities
    pthread_attr_t attr;
    struct sched_param param;

    pthread_attr_init(&attr);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    // Creating threads A, B, and C with priorities 1, 2, and 3 respectively
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
        param.sched_priority = i + 1;
        pthread_attr_setschedparam(&attr, &param);
        pthread_create(&threads[i], &attr, thread_function, (void *)&semaphores[i]);
    }

    // Create timers
    timer_t timers[NUM_THREADS];
    create_timer(&timers[0], 0); // Timer A
    create_timer(&timers[1], 1); // Timer B
    create_timer(&timers[2], 2); // Timer C

    // Set up signals for timers
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = timer_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGRTMIN, &sa, NULL);

    // Set timers
    struct itimerspec its[NUM_THREADS];
    its[0].it_value.tv_sec = 1; // Timeout at 1 second for Thread A
    its[1].it_value.tv_sec = 2; // Timeout at 2 seconds for Thread B
    its[2].it_value.tv_sec = 5; // Timeout at 5 seconds for Thread C
    for (int i = 0; i < NUM_THREADS; ++i) {
        its[i].it_value.tv_nsec = 0;
        its[i].it_interval.tv_sec = 0;
        its[i].it_interval.tv_nsec = 0;
        timer_settime(timers[i], 0, &its[i], NULL);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy semaphores
    for (int i = 0; i < NUM_THREADS; ++i) {
        sem_destroy(&semaphores[i]);
    }

    return 0;
}

void *thread_function(void *arg) {
    sem_t *sem = (sem_t *)arg;
    printf("Start thread %c.\n", (char)(((sem_t*)arg - (sem_t*)semaphores) + 'A'));

    //while (1) {
    for(int i=0; i<5; i++) {
        sem_wait(sem);
        printf("Thread %c is unblocked.\n", (char)(((sem_t*)arg - (sem_t*)semaphores) + 'A'));
        // Do thread-specific operations here
        //sleep(1); // Simulate work
    }

    pthread_exit(NULL);
}

void timer_handler(int signum, siginfo_t *si, void *data) {
    sem_t *sem = (sem_t *)&semaphores[si->si_value.sival_int];
    printf("timer sem_post %p \n", (int*)sem);
    sem_post(sem);
}

void create_timer(timer_t *timer, int timer_id) {
    struct sigevent sev;

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_int = timer_id;
    if (timer_create(CLOCK_REALTIME, &sev, timer) == -1) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }
}
