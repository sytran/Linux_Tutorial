#include <stdio.h>      // Standard Input and Output Library
#include <stdlib.h>     // Standard Library, including memory allocation and process control functions
#include <unistd.h>     // Standard symbolic constants and types, and declarations for POSIX operating system API
#include <fcntl.h>      // File control options, including open(), read(), write(), close()
#include <sys/ioctl.h>  // Input/output control operations, including device-specific input/output control (ioctl) calls
#include <linux/watchdog.h> // Header file for Linux watchdog timer support

int main() {
    int fd;
    int timeout = 10; // Watchdog timeout in seconds

    // Open the watchdog device
    fd = open("/dev/watchdog", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror("Failed to open watchdog device");
        exit(EXIT_FAILURE);
    }

    // Set the watchdog timeout
    if (ioctl(fd, WDIOC_SETTIMEOUT, &timeout) != 0) {
        perror("Failed to set watchdog timeout");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Enable the watchdog
    if (ioctl(fd, WDIOC_ENABLE, NULL) != 0) {
        perror("Failed to enable watchdog");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Main loop to keep resetting the watchdog
    while (1) {
        printf("Resetting watchdog timer...\n");
        if (ioctl(fd, WDIOC_KEEPALIVE, NULL) != 0) {
            perror("Failed to reset watchdog");
            close(fd);
            exit(EXIT_FAILURE);
        }
        sleep(5); // Reset watchdog every 5 seconds
    }

    // Close the watchdog device
    close(fd);
    return 0;
}