/* process_ring.c - process_ring */

/*
 * @author Arun Nekkalapudi
 */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include "process_ring.h"

/*
 * FUNCTION : print_usage
 * DESCRIPTION : Outputs Help Document.
 * ARGUMENTS : char *string1 , char *string2.
 * RETURNS : NA.
 */

void print_usage(void)
{
    printf("Usage: Process Ring\n");
    printf("-p: <no. Of Process> <0-64>\n");
    printf("-r: no. Of Rounds \n");
    printf("-i: Choose Implementation <Poll / Sync>");
    printf("\tPoll : Polling\n");
    printf("\tSync : Semaphore\n");
}

/*
 * FUNCTION : decrementValue_polling
 * DESCRIPTION : POLLING Technique for decrementing values.
 * ARGUMENTS : volatile int32 processIndex.
 * RETURNS : OK -> On Completion.
 */

process decrementValue_polling(volatile int32 *processIndex)
{
    int32 initRounds = 0;
    while (initRounds < rounds)
    {
        int32 processValue = process_polling[*processIndex];
        pollingValueStore[*processIndex] = processValue;
        processIndex++;
        printf("Ring Element %d : Round %d : Value : %d\n", processIndex, initRounds, processValue);
        if (processIndex == processCount - 1)
        {
            initRounds++;
            processIndex = 0;
        }
        else
        {
            processIndex = ProcessIndex + 1;
        }
    }
    return OK;
}

/*
 * FUNCTION : decrementValue_semaphore
 * DESCRIPTION : SEMAPHORE Technique for decrementing values.
 * ARGUMENTS : int32 processIndex.
 * RETURNS : OK -> On Completion.
 */

process decrementValue_semaphore(int32 processIndex)
{
    int32 updatedIndex;
    while (initRounds < rounds)
    {
        wait(process_semaphores[processIndex]);
        if (processValue_semaphore > -1)
        {
            printf("Ring Element %d : Round %d : Value : %d\n", processIndex, initRounds, processValue_semaphore);

            liveCounter = liveCounter + 1;
            processValue_semaphore = processValue_semaphore - 1;

            if (processIndex == processCount - 1)
            {
                updatedIndex = 0;
                initRounds = initRounds + 1;
            }
            else
            {
                updatedIndex = processIndex + 1;
            }
        }
        signal(process_semaphores[updatedIndex]);
    }
    signal(process_doneSemaphores[processIndex]);
    return OK;
}
