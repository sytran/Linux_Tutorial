#include <pthread.h> // Multithreading
#include <semaphore.h>
#include <stdint.h>  // using linux typedef
#include <stdlib.h>  // for atexit()
#include <stdio.h>
#include <termios.h> // For changing terminal mode
#include <unistd.h>  // For changing terminal mode


// ---------------------------------------------
//
#define SLEEP_TIME   3   //number of seconds to sleep  

// ---------------------------------------------
//
struct termios original; // A struct to save the original state of terminal

void disableRAWMode();
void enableRAWMode();

void *ascii_read_thread();
void *timer_event_thread();
void *state_machine_thread();


// ---------------------------------------------
//
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
  GOOD_STATUS = 0,
  STAY_PUT = 1,
  GO_BACK = 2,
  GO_FORWARD = 3,
} Status_t;

Status_t status_global;

// ---------------------------------------------
//
char key_pressed_global = 0;
State_t state_singleton = UNKNOW;

uint32_t cond_index = 0;
int32_t conductivity = 0;
int32_t cond_data[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

uint32_t ph_index = 0;
int32_t phValue = 0;
int32_t ph_data[] = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29};

uint32_t mv_index = 0;
int32_t miliVoltValue = 0;
int32_t mv_data[] = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39};

// ---------------------------------------------

// ---------------------------------------------
// Start Multithreading
//
int main() {

  // allocate objects use in multi thread
  pthread_t id_timer, id_read, id_machine;

  // create all threads running in the system
  pthread_create(&id_timer, NULL, timer_event_thread, NULL);
  pthread_create(&id_read, NULL, ascii_read_thread, NULL);
  pthread_create(&id_machine, NULL, state_machine_thread, NULL);

  // join all threads in the system
  pthread_join(id_timer, NULL);
  pthread_join(id_read, NULL);
  pthread_join(id_machine, NULL);

  return 0;
}

// ---------------------------------------------
//
Status_t service_cond() {
  Status_t status_local = ERROR;
  printf("Service COND State \n");

  switch (key_pressed_global) {
    case LEFT:
      status_local = GO_BACK;
      printf("  case LEFT \n");
    break;

    case RIGHT:
      status_local = GO_FORWARD;
      printf("  case RIGHT \n");
    break;

    default:
      printf(" No New User Event \n");
      status_local = STAY_PUT;
  }
  key_pressed_global = '\n'; // clear message flag
  return status_local;
}

// ---------------------------------------------
//
Status_t service_ph() {
  Status_t status_local = ERROR;
  printf("Service PH State \n");

  switch (key_pressed_global) {
    case LEFT:
      status_local = GO_BACK;
      printf("  case LEFT \n");
    break;

    case RIGHT:
      status_local = GO_FORWARD;
      printf("  case RIGHT \n");
    break;

    default:
      printf(" No New User Event \n");
      status_local = STAY_PUT;
  }
  key_pressed_global = '\n'; // clear message flag
  return status_local;
}


// ---------------------------------------------
//
Status_t service_miliVolt() {
  Status_t status_local = ERROR;
  printf("Service MV State \n");

  switch (key_pressed_global) {
    case LEFT:
      status_local = GO_BACK;
      printf("  case LEFT \n");
    break;

    case RIGHT:
      status_local = GO_FORWARD;
      printf("  case RIGHT \n");
    break;

    default:
      printf(" No New User Event \n");
      status_local = STAY_PUT;
  }
  key_pressed_global = '\n'; // clear message flag
  return status_local;
}


// ---------------------------------------------
//
void *state_machine_thread() {
  Status_t status_local = ERROR;

  for(;;) {
    sleep(SLEEP_TIME);
    printf("run state_machine_thread \n");

    switch (state_singleton) {
      case COND:
        status_local = service_cond();
        switch (status_local) {
          case GO_BACK:
            state_singleton = MV;
          break;

          case GO_FORWARD:
            state_singleton = PH;
          break;

          default: // NO_CHANGE 
            state_singleton = COND;
          break;
        }
      break;

      case PH:
        status_local = service_ph();
        switch (status_local) {
          case GO_BACK:
            state_singleton = COND;
          break;

          case GO_FORWARD:
            state_singleton = MV;
          break;

          default: // NO_CHANGE 
            state_singleton = PH;
          break;
        }
      break;

      case MV:
        status_local = service_miliVolt();
        switch (status_local) {
          case GO_BACK:
            state_singleton = PH;
          break;

          case GO_FORWARD:
            state_singleton = COND;
          break;

          default: // NO_CHANGE 
            state_singleton = MV;
          break;
        }
      break;

      default:
        printf(" State Machine -- state unknown\n");
        state_singleton = COND;
      break;
    }
  }
}


// ---------------------------------------------
// Doing Stuff while listening to keyboard
//
void *timer_event_thread() {
  for(;;) {
    sleep(SLEEP_TIME);
    printf("run timer_event_thread \n");

    switch(state_singleton) {
      case COND:
        conductivity = cond_data[cond_index++ % 10];
        printf("  Read new Cond = %d \n", conductivity);
      break;

      case PH:
        phValue = ph_data[ph_index++ % 10];
        printf("  Read new phValue = %d \n", phValue);
      break;

      case MV:
        miliVoltValue = mv_data[mv_index++ % 10];
        printf("  Read new miliVoltValue = %d \n", miliVoltValue);
      break;

      default:
        printf("Timer Event -- state unknown\n");
        state_singleton = COND;
      break;

    }
  }
}


// ---------------------------------------------
// 
Status_t verify_user_input(char key_entry) {
  Status_t status = GOOD_STATUS;
  uint32_t ascii_key;
  ascii_key = (uint32_t) key_entry;

  switch(ascii_key) {
    case (uint32_t)'a':
      status = GOOD_STATUS;
    break;

    case (uint32_t)'d':
      status = GOOD_STATUS;
    break;

    case (uint32_t)'\n':
      status = GOOD_STATUS;
    break;

    default:
      status = ERROR;
      printf("  Detect Not Supported Key \n");
    break;
  }
  return status;
}

// ---------------------------------------------
// Reads keyboard input
//
void *ascii_read_thread() {
  enableRAWMode(); // local function: Enable Raw Mode
  char key_entry;

  for(;;) {
    sleep(SLEEP_TIME);
    printf("run asciRead \n");

    if ((key_entry = getchar()) != '\n') {  // ignore line return '\n'
      printf("getchar: %c\n", key_entry);

      if(GOOD_STATUS == verify_user_input(key_entry)) {
          key_pressed_global= key_entry;
      }
    }
  }
}

// ---------------------------------------------
// This function enables RAW mode for terminal.
//
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

// ---------------------------------------------
//
void disableRAWMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH,
            &original); // Set terminal to original state
}
