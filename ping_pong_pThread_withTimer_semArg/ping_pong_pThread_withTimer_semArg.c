#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <time.h>

sem_t ping_sem, pong_sem;

// Function for the "ping" thread
void *ping_thread(void *arg) {
    sem_t *ping_sem_arg = (sem_t *)arg;
    int i = 0;
    while (1) {
        //sem_wait(&ping_sem);
        sem_wait(ping_sem_arg);
        printf("Ping thread A: Loop %d\n", ++i);
    }
    pthread_exit(NULL);
}

// Function for the "pong" thread
void *pong_thread(void *arg) {
    sem_t *pong_sem_arg = (sem_t *)arg;
    int i = 0;
    while (1) {
        //sem_wait(&pong_sem);
        sem_wait(pong_sem_arg);
        printf("Pong thread B: Loop %d\n", ++i);
    }
    pthread_exit(NULL);
}

// Timer callback function for "ping" timer
void ping_timer_handler(sigval_t value) {
    sem_post(&ping_sem);
}

// Timer callback function for "pong" timer
void pong_timer_handler(sigval_t value) {
    sem_post(&pong_sem);
}

int main() {
    int32_t ping_priority = 10;
    int32_t pong_priority = 20;
	pthread_t ping_tid, pong_tid;
    pthread_attr_t ping_attr, pong_attr;
    struct sched_param param;

    // Initialize semaphores
    sem_init(&ping_sem, 0, 0);
    sem_init(&pong_sem, 0, 0);


    // Initialize thread attributes
    pthread_attr_init(&ping_attr);       //initialized with default attributes
    pthread_attr_getschedparam (&ping_attr, &param);  // get existing scheduling param
    param.sched_priority = ping_priority;
    pthread_attr_setschedparam (&ping_attr, &param);  // update the thread priority
    pthread_attr_init(&ping_attr);

    pthread_attr_init(&pong_attr);
    pthread_attr_getschedparam (&pong_attr, &param);
    param.sched_priority = pong_priority;
    pthread_attr_setschedparam (&pong_attr, &param);
    pthread_attr_init(&pong_attr);

    // Create "ping" thread
    pthread_create(&ping_tid, &ping_attr, ping_thread, (void *)&ping_sem);

    // Create "pong" thread
    pthread_create(&pong_tid, &pong_attr, pong_thread, (void *)&pong_sem);

    // Set up timers
    timer_t ping_timer, pong_timer;
    struct sigevent ping_sev, pong_sev;

    // Configure "ping" timer
    ping_sev.sigev_notify = SIGEV_THREAD;
    ping_sev.sigev_notify_function = ping_timer_handler;
    ping_sev.sigev_value.sival_ptr = NULL;
    timer_create(CLOCK_REALTIME, &ping_sev, &ping_timer);

    // Configure "pong" timer
    pong_sev.sigev_notify = SIGEV_THREAD;
    pong_sev.sigev_notify_function = pong_timer_handler;
    pong_sev.sigev_value.sival_ptr = NULL;
    timer_create(CLOCK_REALTIME, &pong_sev, &pong_timer);

    // Set timer intervals
    struct itimerspec ping_ts, pong_ts;
    ping_ts.it_value.tv_sec = 1;
    ping_ts.it_value.tv_nsec = 0;
    ping_ts.it_interval.tv_sec = 1;
    ping_ts.it_interval.tv_nsec = 0;
    timer_settime(ping_timer, 0, &ping_ts, NULL);

    pong_ts.it_value.tv_sec = 2;
    pong_ts.it_value.tv_nsec = 0;
    pong_ts.it_interval.tv_sec = 2;
    pong_ts.it_interval.tv_nsec = 0;
    timer_settime(pong_timer, 0, &pong_ts, NULL);

    // Wait for threads to finish (not reachable in infinite loops)
    pthread_join(ping_tid, NULL);
    pthread_join(pong_tid, NULL);

    // Destroy semaphores
    sem_destroy(&ping_sem);
    sem_destroy(&pong_sem);

    // Destroy thread attributes
    pthread_attr_destroy(&ping_attr);
    pthread_attr_destroy(&pong_attr);

    return 0;
}
