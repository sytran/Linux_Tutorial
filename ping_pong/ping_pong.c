#include <pthread.h> // Multithreading
#include <stdio.h>   // for printf()
#include <stdint.h>  // for int32_t
#include <stdlib.h>  //for malloc()
#include <unistd.h>  // for sleep()

int32_t ping_function(int32_t max_count_arg);
int32_t pong_function(int32_t max_count_arg);

// ------------------------------------------------------------
//
int main() 
{
  int32_t max_loop = 5;

  int32_t ping_return = ping_function(max_loop);
  int32_t pong_return = pong_function(max_loop);

  printf("ping thread returns %d\n", ping_return);
  printf("pong thread returns %d\n", pong_return);
  return 0;
}


// ------------------------------------------------------------
//
int32_t ping_function(int32_t max_count_arg)
{
  int32_t max_count = max_count_arg;
  int32_t ping_return = 5;

  for(int32_t i=1; i <= max_count; i++)
  { 
    printf("running ping function count %d\n", i);
    sleep(0.5);
  }

  printf("ping_thread Finished!\n");

  return ping_return;
}

// ------------------------------------------------------------
//
int32_t pong_function(int32_t max_count_arg)
{
  int32_t max_count = max_count_arg;
  int32_t pong_return = 10;

  for(int32_t i=1; i <= max_count; i++)
  { 
    printf("running pong function count %d\n", i);
    sleep(1);
  }

  printf("pong_thread Finished!\n");

  return pong_return;
}


