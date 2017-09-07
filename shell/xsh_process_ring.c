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
    initRounds = 0;
    liveCounter = 0;
    /*3 Process 3 Rounds*/
    processCount = 4;
    rounds = 4;

    /*Start Of Argument Processing*/
    // int32 p = 2; /* specified default */
    // char *endp;
    // int32 j;
    // for (j = 1; j < argc; j++)
    // {
    //     if (0 == strncmp("-p", args[j], 3))
    //     {
    //         /* parse numeric argument to -p */
    //         if (!(j + 1 < argc))
    //         {
    //             /* No argument after flag */
    //             print_usage();
    //             printf("-p flag expected an argument\n");
    //             return SHELL_ERROR;
    //         }
    //         p = strtol(args[j + 1], &endp, 10);
    //         if (endp == args[j + 1])
    //         {
    //             /* The end pointer hasn't advanced,
    //          No number was parsed.*/
    //             print_usage();
    //             printf("-p flag expected an integer\n");
    //             return SHELL_ERROR;
    //         }
    //         else if (*endp != '\0')
    //         {
    //             /* There was trailing garbage in the string that
    //         wasn't converted to an integer. */
    //             print_usage();
    //             printf("-p recieved invalid integer\n");
    //             return SHELL_ERROR;
    //         }
    //         else if (!(0 <= p && p <= 64))
    //         {
    //             /* The number is out of range */
    //             print_usage();
    //             printf("-p flag expected a number between 0 - 64\n");
    //             return SHELL_ERROR;
    //         }
    //         /* Skip pass the numeric argument,
    //        that was successfully parsed */
    //         j += 1;
    //     }
    //     else /* if (...) */
    //     {
    //         // Handle other cases
    //     }
    // }
    // printf("argument  is %d\n", p);
    // return SHELL_OK;
    /*End of Srgument Processing*/

    
    
    /*Polling*/
    /*if (processCount > 0 && processCount < 64 && rounds > 0)
    {
        process_polling[0] = (processCount * rounds) - 1;
    }
    for (i = 0; i < processCount; i++)
    {
        resume(create(decrementValue_polling, 1024, 20, (char)i, 1, i));
    }
    while (initRounds < rounds);
    */
    /*End of polling Block*/

    /*Semaphores*/
    if (processCount > 0 && processCount < 64 && rounds > 0)
    {
        for(i=0;i<processCount;i++){
            if(i==0){
                process_semaphores[i] = semcreate(1);
            }    
            else{
                process_semaphores[i] = semcreate(0);
            }    
                process_doneSemaphores[i] =semcreate(0);
        }
        processValue_semaphore = (processCount * rounds) - 1;
    }

    for (i = 0; i < processCount; i++)
    {
        resume(create(decrementValue_semaphore, 1024, 20, (char)i, 1, i));
    }

    for(i=0;i<processCount;i++){
        wait(process_doneSemaphores[i]);
        semdelete(process_semaphores[i]); 
	semdelete(process_doneSemaphores[i]);
    }

    /*End of Semaphore Block*/
    
    return SHELL_OK;
}
