/* xsh_process_ring.c - xsh_process_ring */

/*
 * @author Arun Nekkalapudi
 */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "process_ring.h"

/*------------------------------------------------------------------------
  * Process_Ring- Using Polling & Semaphores.
  *-----------------------------------------------------------------------
  */

shellcmd xsh_process_ring(int nargs, char *args[])
{
    volatile int32 index = 0;
    int32 i;
    initRounds = 0;
    liveCounter = 0;
    int32 retval;		
    int32 retval1;	
	uint32	now;	

    /*Start Of Argument Processing*/
    processCount = 2; /* specified default */
    rounds = 3;
    int32 impNumber = 0;
    int32 j;
    for (j = 1; j < nargs; j++)
    {
        if (strcmp("--help", args[j]) == 0)
        {
            print_usage();
            return SHELL_OK;
        }

        if (strcmp("-p", args[j]) == 0)
        {
            /* parse numeric argument to -p */
            if (!(j + 1 < nargs))
            {
                /* No argument after flag */
                print_usage();
                printf("-p flag expected an argument\n");
                return SHELL_ERROR;
            }
            processCount = atoi(args[j + 1]);
            if (!(0 <= processCount && processCount <= 64))
            {
                print_usage();
                printf("-p flag expected a number between 0 - 64\n");
                return SHELL_ERROR;
            }
        }
        if (strcmp("-r", args[j]) == 0)
        {
            /* parse numeric argument to -r */
            if (!(j + 1 < nargs))
            {
                print_usage();
                printf("-r flag expected an argument\n");
                return SHELL_ERROR;
            }
            rounds = atoi(args[j + 1]);
            if (!(0 < rounds && rounds <= 100))
            {
                print_usage();
                printf("-r flag expected a number between 0 - 100\n");
                return SHELL_ERROR;
            }
        }
        if (strcmp("-i", args[j]) == 0)
        {
            if (strcmp(args[j + 1], "poll") == 0)
            {
                //print_usage();
                impNumber = 0;
                //printf("Polling has been enabled");
            }
            else if (strcmp(args[j + 1], "sync") == 0)
            {
                //print_usage();
                impNumber = 1;
                //printf("Sync");
            }
            else
            {
                print_usage();
            }
        }
        //else /* if (...) */
        //{
        //    // Handle other cases
        //}
    }
    printf("Number of Processes: %d\n",processCount);
    printf("Number of Rounds: %d\n",rounds);
    
    retval = gettime(&now)
    /*End of Argument Processing*/
    if (impNumber == 0)
    {
        /*Polling*/
        if (processCount > 0 && processCount < 64 && rounds > 0)
        {
            pollingValueStore[0] = (processCount * rounds) - 1;
        }
        for (i = 0; i < processCount; i++)
        {
            resume(create(decrementValue_polling, 1024, 20, "process", 1, &index));
        }
        while (index < (processCount * rounds))
            ;
        /*End of polling Block*/
    }

    if (impNumber == 1)
    {
        /*Semaphores*/
        if (processCount > 0 && processCount < 64 && rounds > 0)
        {
            for (i = 0; i < processCount; i++)
            {
                if (i == 0)
                {
                    process_semaphores[i] = semcreate(1);
                }
                else
                {
                    process_semaphores[i] = semcreate(0);
                }
                process_doneSemaphores[i] = semcreate(0);
            }
            processValue_semaphore = (processCount * rounds) - 1;
        }
        for (i = 0; i < processCount; i++)
        {
            resume(create(decrementValue_semaphore, 1024, 20, "process", 1, i));
        }

        for (i = 0; i < processCount; i++)
        {
            wait(process_doneSemaphores[i]);
            semdelete(process_semaphores[i]);
            semdelete(process_doneSemaphores[i]);
        }
        /*End of Semaphore Block*/
    }
    retval1 = gettime(&now)
    printf("Elapsed Seconds: %d\n",retval1-retval);

    return SHELL_OK;
}
