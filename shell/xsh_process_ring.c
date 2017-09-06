/* xsh_process_ring.c - xsh_process_ring */

/*
 * @author Arun Nekkalapudi
 */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include "process_ring.h"

/*------------------------------------------------------------------------
  * Process_Ring- Using Polling & Semaphores.
  *------------------------------------------------------------------------
  */


shellcmd xsh_process_ring(int nargs, char *args[])
{
    int32 i;

    /*3 Process 3 Rounds*/
    processCount = 3;
    rounds = 3;

    if(processCount>0 && processCount<64 && rounds > 0){
        process_polling[0] = (processCount*rounds) - 1;
    }

    for(i=0;i<processCount;i++){
        resume(create(decrementValue, 1024, 20,(char)i, 1,i));
    }

    while(initRounds<rounds);
    return SHELL_OK;
}

