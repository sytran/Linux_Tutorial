#include <pthread.h> // Multithreading
#include <stdio.h>  // for printf()
#include <stdint.h> // for int32_t
#include <stdlib.h> // for malloc()
#include <unistd.h> // for sleep()
#include <sched.h>  // use thread priority

void * ping_thread(void *max_count_pointer);
void * pong_thread(void *max_count_pointer);

// ------------------------------------------------------------
//
int main() 
{
  // Start Multithreading
  pthread_t id_task_ping;
  pthread_t id_task_pong;

  pthread_attr_t thread_attribute;
  struct sched_param   param;
  int32_t ping_priority = 10;
  int32_t pong_priority = 20;
  
  int32_t *ping_return; 
  int32_t *pong_return; 

  int32_t max_loop = 10;

  pthread_attr_init(&thread_attribute);       //initialized with default attributes
  pthread_attr_getschedparam (&thread_attribute, &param);  // get existing scheduling param

  param.sched_priority = ping_priority;          
  pthread_attr_setschedparam (&thread_attribute, &param);  // update the thread priority
  pthread_create(&id_task_ping, &thread_attribute, ping_thread, &max_loop); 

  param.sched_priority = pong_priority;          
  pthread_attr_setschedparam (&thread_attribute, &param);  // update the thread priority
  pthread_create(&id_task_pong, NULL, pong_thread, &max_loop);

  pthread_join(id_task_ping, (void*)&ping_return);
  pthread_join(id_task_pong, (void*)&pong_return);

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


