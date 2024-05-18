#include <pthread.h> // Multithreading
#include <stdio.h>
#include <stdlib.h>  // for atexit()
#include <termios.h> // For changing terminal mode
#include <unistd.h>  // For changing terminal mode
#include <stdint.h>

struct termios original; // A struct to save the original state of terminal
int ESCPressed = 0;      // For thread communication

void disableRAWMode();
void enableRAWMode();
void *ascii_read_thread();
void *timer_event_thread();
void *state_machine_thread();

typedef enum state_define_e {
  UNKNOW,
  COND,
  PH,
  MV,
} State_t;

typedef enum key_event_e {
  LEFT = 'a' ,  // a
  RIGHT = 'd' ,  // d
} Key_event_t;

typedef enum status_e {
  ERROR = -1,
  GO_BACK = 1,
  GO_FORWARD = 2,
} Status_t;

Status_t status_global;

char key_pressed = 0;
State_t state = UNKNOW;
uint32_t i = 0;
int32_t conductivity = 0;
int32_t cond_data[] = { 1, 2, 3, 4, 5};

int32_t phValue = 0;
int32_t ph_data[] = { 11, 12, 13, 14, 15};

int32_t miliVoltValue = 0;
int32_t mv_data[] = { 21, 22, 23, 24, 25};

// ---------------------------------------------
//
int main() {
  // Start Multithreading
  pthread_t id_timer, id_read, id_machine;

  pthread_create(&id_timer, NULL, timer_event_thread, NULL);
  pthread_create(&id_read, NULL, ascii_read_thread, NULL);
  pthread_create(&id_machine, NULL, state_machine_thread, NULL);

  pthread_join(id_timer, NULL);
  pthread_join(id_read, NULL);
  pthread_join(id_machine, NULL);

  return 0;
}

// ---------------------------------------------
//
Status_t service_cond() {
  Status_t status_local = -1;

  switch (key_pressed) {
    case LEFT:
      status_local = GO_BACK;
      printf("service_cond LEFT \n");
    break;

    case RIGHT:
      status_local = GO_FORWARD;
      printf("service_cond RIGHT \n");
    break;

    default:
      status_local = ERROR;
  }

  return status_local;
}

// ---------------------------------------------
//
Status_t service_ph() {
  Status_t status_local = -1;

  switch (key_pressed) {
    case LEFT:
      status_local = GO_BACK;
      printf("service_ph LEFT \n");
    break;

    case RIGHT:
      status_local = GO_FORWARD;
            printf("service_ph RIGHT \n");
    break;

    default:
      status_local = ERROR;
  }

  return status_local;
}


// ---------------------------------------------
//
Status_t service_miliVolt() {
  Status_t status_local = -1;

  switch (key_pressed) {
    case LEFT:
      status_local = GO_BACK;
      printf("service_miliVolt LEFT \n");
    break;

    case RIGHT:
      status_local = GO_FORWARD;
      printf("service_miliVolt RIGTH \n");
    break;

    default:
      status_local = ERROR;
  }

  return status_local;
}

// ---------------------------------------------
//
void *state_machine_thread() {
  Status_t status_local = -1;

  for(;;) {
    sleep(2);
    printf("run state_machine_thread \n");

    switch (state) {
      case COND:
        status_local = service_cond();
        switch (status_local) {
          case GO_BACK:
            state = MV;
          break;

          case GO_FORWARD:
            state = PH;
          break;

          default: // NO_CHANGE 
            state = COND;
          break;
        }
      break;

      case PH:
        status_local = service_ph();
        switch (status_local) {
          case GO_BACK:
            state = COND;
          break;

          case GO_FORWARD:
            state = MV;
          break;

          default: // NO_CHANGE 
            state = PH;
          break;
        }
      break;

      case MV:
        status_local = service_miliVolt();
        switch (status_local) {
          case GO_BACK:
            state = PH;
          break;

          case GO_FORWARD:
            state = COND;
          break;

          default: // NO_CHANGE 
            state = MV;
          break;
        }
      break;

      default:
        printf(" State Machine -- state unknown\n");
        state = COND;
      break;
    }
  }
}

// ---------------------------------------------
// Reads keyboard input
//
void *ascii_read_thread() {
  enableRAWMode(); // local function: Enable Raw Mode
  char ch;

  for(;;) {
    sleep(2);
    printf("run ascii_read_thread \n");

    if ((ch = getchar()) != 27) {  // ASCI code for ESC is 27
      printf("getchar: %c\n", ch);
    }
    key_pressed = ch;
  }

  //ESCPressed = 1;
  //printf("ESC Pressed!\n");

}

// ---------------------------------------------
// Doing Stuff while listening to keyboard
//
void *timer_event_thread() {
  while (!ESCPressed) { // When ESC is not pressed
    sleep(2);
    printf("run timer_event_thread \n");

    switch(state) {
      case COND:
        conductivity = cond_data[i++ % 5];
        printf("Read new Cond = %d \n", conductivity);
      break;

      case PH:
        phValue = ph_data[i++ % 5];
        printf("Read new phValue = %d \n", phValue);
      break;

      case MV:
        miliVoltValue = mv_data[i++ % 5];
        printf("Read new miliVoltValue = %d \n", miliVoltValue);
      break;

      default:
        printf("Timer Event -- state unknown\n");
        state = MV;
      break;

    }

  }
  printf("Printing Thread Finished!\n");
}

/// This function enables RAW mode for terminal.
void enableRAWMode() {
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw); // Save the state of the terminal to struct raw
                                 // STDIN_FILENO is from <stdlib.h>
                                 // tcgetattr() from <termios.h>
  tcgetattr(STDIN_FILENO, &original);
  atexit(&disableRAWMode); // Revert to canonical mode when exiting the program
                           // atext() from <stdlib.h>
  raw.c_lflag &=
      ~(ECHO | ICANON); // Turn off canonical mode
                        // Turn off ECHO mode so that keyboard is not
                        // printing to terminal
                        // ICANON and ECHO is bitflag. ~ is binary NOT operator

  tcsetattr(STDIN_FILENO, TCSAFLUSH,
            &raw); // Set the terminal to be in raw mode
                   // tcsetattr() from <termios.h>
}

void disableRAWMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH,
            &original); // Set terminal to original state
}
