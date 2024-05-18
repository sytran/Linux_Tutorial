#include <stdio.h>      // Standard Input and Output Library
#include <stdlib.h>     // Standard Library, including memory allocation and process control functions
#include <pthread.h>    // POSIX threads library for multi-threading
#include <unistd.h>     // Standard symbolic constants and types, and declarations for POSIX operating system API
#include <fcntl.h>      // File control options, including open(), read(), write(), close()
#include <sys/ioctl.h>  // Input/output control operations, including device-specific input/output control (ioctl) calls
#include <linux/watchdog.h> // Header file for Linux watchdog timer support
#include <signal.h>     // Signal handling library

// Watchdog file descriptor
int watchdog_fd;

// Function to pat the watchdog
void pat_watchdog() {
    if (ioctl(watchdog_fd, WDIOC_KEEPALIVE, NULL) != 0) {
        perror("Failed to reset watchdog");
        exit(EXIT_FAILURE);
    }
}

// Signal handler for SIGALRM (watchdog timeout)
void handle_watchdog_timeout(int signum) {
    printf("Watchdog timeout occurred. System is about to reboot.\n");
    // You can add custom actions to be performed when watchdog timeout occurs
    // For example, you can log the event, perform cleanup, etc.
    exit(EXIT_FAILURE);
}

// Thread A function
void* thread_A(void* arg) {
    while (1) {
        printf("Thread A patting the watchdog...\n");
        pat_watchdog();
        sleep(1); // Sleep for 1 second
    }
    return NULL;
}

// Thread B function
void* thread_B(void* arg) {
    while (1) {
        printf("Thread B patting the watchdog...\n");
        pat_watchdog();
        sleep(2); // Sleep for 2 seconds
    }
    return NULL;
}

int main() {
    // Open the watchdog device
    watchdog_fd = open("/dev/watchdog", O_RDWR | O_NOCTTY);
    if (watchdog_fd == -1) {
        perror("Failed to open watchdog device");
        exit(EXIT_FAILURE);
    }

    // Set the watchdog timeout to 3 seconds
    int timeout = 3;
    if (ioctl(watchdog_fd, WDIOC_SETTIMEOUT, &timeout) != 0) {
        perror("Failed to set watchdog timeout");
        close(watchdog_fd);
        exit(EXIT_FAILURE);
    }

    // Register signal handler for SIGALRM (watchdog timeout)
    signal(SIGALRM, handle_watchdog_timeout);

    // Create threads A and B
    pthread_t tid_A, tid_B;
    if (pthread_create(&tid_A, NULL, thread_A, NULL) != 0 ||
        pthread_create(&tid_B, NULL, thread_B, NULL) != 0) {
        perror("Failed to create threads");
        close(watchdog_fd);
        exit(EXIT_FAILURE);
    }

    // Join threads A and B (this will never be reached in this example)
    pthread_join(tid_A, NULL);
    pthread_join(tid_B, NULL);

    // Close the watchdog device
    close(watchdog_fd);

    return 0;
}
