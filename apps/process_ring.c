#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include "process_ring.h"

/*
 * @author Arun Nekkalapudi
 */ 


process decrementValue_polling(volatile int32 processIndex)
{
    int32 tmp;
    while(initRounds<rounds){
        int32 processValue = process1[processIndex] ;
        printf("Ring Element %d : Round %d : Value : %d\n",processIndex,initRounds,processValue);
        if(processIndex == processCount-1){
            tmp = 0;
            processIndex = tmp;
            initRounds = initRounds + 1;
        }
        else{
            tmp  = processIndex + 1;
            processIndex = tmp;
        }
        process1[tmp] = processValue - 1;
    }
    return OK;
}
