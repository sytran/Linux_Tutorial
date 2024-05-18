#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

// Function to be called when timer expires
void timer_callback(int signum) {
    printf("Timer expired. Do something here...\n");
}

int main() {
    struct sigevent sev;
    timer_t timerid;
    struct itimerspec its;

    // Set up the signal handler
    signal(SIGALRM, timer_callback);

    // Create a timer
    sev.sigev_notify = SIGEV_SIGNAL;   // Notify via signal
    sev.sigev_signo = SIGALRM;         // Signal to deliver upon timer expiration
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    // Set the timer to expire after 2 seconds, then every 2 seconds after that
    its.it_value.tv_sec = 2;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 2;
    its.it_interval.tv_nsec = 0;

    // Start the timer
    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }

    // Loop indefinitely
    while (1) {
        // Do other work here while waiting for the timer to expire
    }

    return 0;
}
