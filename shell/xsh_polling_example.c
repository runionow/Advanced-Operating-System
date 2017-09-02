/* xsh_create_example.c - xsh_create_example */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_create_example - print a familiar and friendly greeting
 *------------------------------------------------------------------------
 */

process polling_consumer(volatile int32 *produce, volatile int32 *consume){
  volatile int32 last = -1;
  while (last < 3){                                                              
    /* Polling to see if the producer has produced a new value*/
    int32 tmp = *produce;                                                       
    //printf("consumer polling: %d\n", tmp);                                                        
    if (last != tmp){
      printf("consumed: %d\n", tmp);
      last = tmp;
      *consume = tmp;
    }                                                                           
  }                                                                               
  return OK;                                                                      
}

process polling_producer(int32 *produce, volatile int32 *consume){                                  
  int32 last = -1;                                                                
  while (last < 3){                                                              
    /* Polling to see if the consumer has consumed the last value */
    int32 tmp = *consume;                                                       
    //printf("producer polling: %d\n", tmp);                                                        
    if (last == tmp){
      printf("produced: %d\n", tmp + 1);
      last = tmp + 1;
      *produce = tmp + 1;
    }                                                                           
  }                                                                               
  return OK;                                                                      
}

shellcmd xsh_polling_example(int nargs, char *args[]) {
  int32 produce = -1;                                                    
  volatile int32 consume = -1;
  resume(create(polling_producer, 1024, 20, "producer", 2,                
                &produce, &consume));                                             
  resume(create(polling_consumer, 1024, 20, "consumer", 2,                
                &produce, &consume));                                             
  /* Polling to see if both producer and consumer are done */
  while (consume < 3){
    //printf("waiting: %d\n", consume);
  }
  return SHELL_OK;
}

