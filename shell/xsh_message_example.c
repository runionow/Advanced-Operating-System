/* xsh_create_example.c - xsh_create_example */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_create_example - print a familiar and friendly greeting
 *------------------------------------------------------------------------
 */

process msg_consumer(pid32 parent_pid){
  int32 last = -1; 
  while (last < 3){
    last = receive();
    printf("consumed: %d\n", last);
  }
  send(parent_pid, OK);
  return OK;
}                    

process msg_producer(pid32 consume_pid,
                     pid32 parent_pid){
  int32 last = -1; 
  while (last < 3){
    last = last + 1;
    printf("produced: %d\n", last);
    send(consume_pid, last);
  }
  send(parent_pid, OK);
  return OK;
}                         

shellcmd xsh_message_example(int nargs, char *args[]) {
  pid32 parent_pid  = getpid();
  pid32 consume_pid = create(msg_consumer, 1024, 20, "consumer", 1, parent_pid);
  resume(consume_pid);
  resume(create(msg_producer, 1024, 20, "producer", 2,
                consume_pid, parent_pid));
  receive();
  receive();
  return SHELL_OK;
}

