/* xsh_polling_example.c - xsh_polling_example */

/*
 * @author Arun Nekkalapudi
 */ 

 #include <xinu.h>
 #include <string.h>
 #include <stdio.h>
 
 /*------------------------------------------------------------------------
  * Polling-Example
  *------------------------------------------------------------------------
  */
  

 process polling_consumer(volatile int32 *produce, volatile int32 *consume){
     volatile int32 last = -1;
     printf("It came to polling_consumer here to execute")
     while(last < 3){
         /* Polling to see if the producer has produced a new value*/
         int32 tmp = *produce;
         if(last != tmp){
             printf("consumed %d\n",tmp);
             last = tmp;
             *consume = tmp;
         }
     }
     return OK;
 }

 process polling_producer(int32 *produce, volatile int32 *consume){
    int32 last = -1;
    printf("It came to polling_producer here to execute")
    while(last < 3){
        /* Polling to see if the consumer has consumed the last value*/
        int32 tmp = *consume;
        if(last == tmp){
            printf("produced %d\n",tmp+1);
            last = tmp+1;
            *produce = tmp+1;
        }
    }
    return OK;
}


 shellcmd xsh_create_example(int nargs, char *args[]) {
 
     /* Create_example*/
     /* This process has the lower priority than the parent process*/
     resume(create(print_it,1024,1,"after",2,21,3));

     /* This process has the higher priority than the parent process*/
     resume(create(print_it,1024,30,"after",2,42,10));
     
     /*I should keep the synchronization to prevent the parent from exiting*/
     return SHELL_OK;

    }
 