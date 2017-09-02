/* xsh_create_example.c - xsh_create_example */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_create_example - print a familiar and friendly greeting
 *------------------------------------------------------------------------
 */

process sem_consumer(sid32 produce_sem,
                     sid32 consume_sem,
                     sid32 done_sem,
                     volatile int32 *produce){
  int32 last = -1;
  while (last < 3){
    /* wait for the producer to produce a new value*/
    // printf("consumer waiting\n");
    wait(consume_sem);
    last = *produce;
    signal(produce_sem);
    printf("consumed: %d\n", last);
  }
  signal(done_sem);
  return OK;
}                    

process sem_producer(sid32 produce_sem,
                     sid32 consume_sem,
                     sid32 done_sem,
                     int32 *produce){                                  
  int32 last = -1;
  while (last < 3){
    last = last + 1;
    printf("produced: %d\n", last);
    //printf("producer waiting\n");
    wait(produce_sem);
    *produce = last;
    signal(consume_sem);
  }
  signal(done_sem);
  return OK;
}                         

shellcmd xsh_semaphore_example(int nargs, char *args[]) {
  sid32 produce_sem = semcreate(1); /* Produce gets to go for free */
  sid32 consume_sem = semcreate(0);
  sid32 done_sem = semcreate(0);
  int32 produce = -1;
  resume(create(sem_producer, 1024, 20, "producer", 4,
                produce_sem, consume_sem, done_sem, &produce));
  resume(create(sem_consumer, 1024, 20, "consumer", 4,
                produce_sem, consume_sem, done_sem, &produce));
  /* Wait for both children to exit */
  //printf("parent waiting\n");
  wait(done_sem);
  wait(done_sem);
  semdelete(produce_sem);
  semdelete(consume_sem);
  semdelete(done_sem);
  return SHELL_OK;
}

