/* xsh_polling_example.c - xsh_polling_example */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_polling_example - pass state between two thread using polling to
 * prevent data races. 
 *------------------------------------------------------------------------
 */

/* Print out new values in the produce location, and acknowledge that
   they were received by placing them in the consume location */
process polling_consumer(volatile int32 *produce, int32 *consume){
  int32 last = -1;
  while (last < 3){
    /* Polling to see if the producer has produced a new value*/
    int32 tmp = *produce;
    if (last != tmp){
      printf("consumed: %d\n", tmp);
      last = tmp;
      *consume = tmp;
    }
  }
  return OK;
}

/* When the last value has been acknowledged produce a new value by
   incrementing the last value and putting it in the produce
   location. */
process polling_producer(int32 *produce, volatile int32 *consume){
  int32 last = -1;
  while (last < 3){
    /* Polling to see if the consumer has consumed the last value */
    int32 tmp = *consume;
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
  while (consume < 3);
  return SHELL_OK;
}

