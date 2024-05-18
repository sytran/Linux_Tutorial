#include <pthread.h> // Multithreading
#include <stdio.h>   // for printf()
#include <stdint.h>  // for int32_t
#include <stdlib.h>  //for malloc()
#include <unistd.h>  // for sleep()

void * ping_thread(void *max_count_pointer);
void * pong_thread(void *max_count_pointer);

// ------------------------------------------------------------
//
int main() 
{
  // Start Multithreading
  pthread_t id_task_ping;
  pthread_t id_task_pong;

  int32_t *ping_return; 
  int32_t *pong_return; 

  int32_t max_loop = 5;

  pthread_create(&id_task_ping, NULL, ping_thread, &max_loop);
//  pthread_create(&id_task_pong, NULL, pong_thread, &max_loop);
  pong_thread(&max_loop);

  pthread_join(id_task_ping, (void*)&ping_return);
  //pthread_join(id_task_pong, (void*)&pong_return);

  printf("ping thread returns %d\n", *ping_return);
  printf("pong thread returns %d\n", *pong_return);

  return 0;
}


// ------------------------------------------------------------
//
void* ping_thread(void * max_count_pointer) 
{
  int32_t *max_count = (int32_t *)max_count_pointer;
  int32_t *ping_return = (int32_t*) malloc(sizeof(int32_t));
  *ping_return = 5;

  for(int32_t i=1; i <= *max_count; i++)
  { 
    printf("running ping_thread count %d\n", i);
    sleep(1);
  }

  printf("ping_thread Finished!\n");

  return ping_return;
}

// ------------------------------------------------------------
//
void* pong_thread(void * max_count_pointer) 
{
  int32_t *max_count = (int32_t *)max_count_pointer;
  int32_t *pong_return = (int32_t*) malloc(sizeof(int32_t));
  *pong_return = 10;

  for(int32_t i=1; i <= *max_count; i++)
  { 
    printf("running pong_thread count %d\n", i);
    sleep(2);
  }

  printf("pong_thread Finished!\n");

  return pong_return;
}


