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
  *-----------------------------------------------------------------------
  */

shellcmd xsh_process_ring(int nargs, char *args[])
{
    // volatile int32 index = 0;
    // int32 i;
    // initRounds = 0;
    // liveCounter = 0;
    
    /*3 Process 3 Rounds*/
    processCount = 10;
    rounds = 4;

    /*Start Of Argument Processing*/
    int32 proc = 2; /* specified default */
    int32 roun = 2; 
    int32 imp = "poll"
    char *endp;
    int32 j;
    for (j = 1; j < nargs; j++)
    {    
        if (0 == strncmp("-p", args[j], 3))
        {
            /* parse numeric argument to -p */
            if (!(j + 1 < nargs))
            {
                /* No argument after flag */
                print_usage();
                printf("-p flag expected an argument\n");
                return SHELL_ERROR;
            }
            proc = strtol(args[j + 1], &endp, 10);
            if (endp == args[j + 1])
            {
                /* The end pointer hasn't advanced,
             No number was parsed.*/
                print_usage();
                printf("-p flag expected an integer\n");
                return SHELL_ERROR;
            }
            else if (*endp != '\0')
            {
                /* There was trailing garbage in the string that
            wasn't converted to an integer. */
                print_usage();
                printf("-p recieved invalid integer\n");
                return SHELL_ERROR;
            }
            else if (!(0 <= p && p <= 64))
            {
                /* The number is out of range */
                print_usage();
                printf("-p flag expected a number between 0 - 64\n");
                return SHELL_ERROR;
            }
            /* Skip pass the numeric argument,
           that was successfully parsed */
            j += 1;
        }
        if (0 == strncmp("-r", args[j], 3))
        {
            /* parse numeric argument to -r */
            if (!(j + 1 < nargs))
            {
                /* No argument after flag */
                print_usage();
                printf("-r flag expected an argument\n");
                return SHELL_ERROR;
            }
            roun = strtol(args[j + 1], &endp, 10);
            if (endp == args[j + 1])
            {
                /* The end pointer hasn't advanced,
             No number was parsed.*/
                print_usage();
                printf("-r flag expected an integer\n");
                return SHELL_ERROR;
            }
            else if (*endp != '\0')
            {
                /* There was trailing garbage in the string that
            wasn't converted to an integer. */
                print_usage();
                printf("-r recieved invalid integer\n");
                return SHELL_ERROR;
            }
            else if (!(0 <= r && r <= 100))
            {
                /* The number is out of range */
                print_usage();
                printf("-r flag expected a number between 0 - 64\n");
                return SHELL_ERROR;
            }
            /* Skip pass the numeric argument,
           that was successfully parsed */
            j += 1;
        }
        if (0 == strncmp("-i", args[j], 3))
        {
            /* parse numeric argument to -i */
            if (!(j + 1 < nargs))
            {
                /* No argument after flag */
                print_usage();
                printf("-i flag expected an argument\n");
                return SHELL_ERROR;
            }
            imp = strtol(args[j + 1], &endp, 10);
            if (endp == args[j + 1])
            {
                /* The end pointer hasn't advanced,
             No number was parsed.*/
                print_usage();
                printf("-i flag expected an integer\n");
                return SHELL_ERROR;
            }
            else if (*endp != '\0')
            {
                /* There was trailing garbage in the string that
            wasn't converted to an integer. */
                print_usage();
                printf("-i recieved invalid integer\n");
                return SHELL_ERROR;
            }
            else if (!(0 <= p && p <= 64))
            {
                /* The number is out of range */
                print_usage();
                printf("-i flag expected a number between 0 - 64\n");
                return SHELL_ERROR;
            }
            /* Skip pass the numeric argument,
           that was successfully parsed */
            j += 1;
        }
        else /* if (...) */
        {
            // Handle other cases
        }
    }
    printf("Process Count %d\n Round %d\n Implementation %s\n", proc,roun,imp);

    return SHELL_OK;

    /*End of Srgument Processing*/

    /*Polling*/
    // if (processCount > 0 && processCount < 64 && rounds > 0)
    // {
    //     pollingValueStore[0] = (processCount * rounds) - 1;
    // }
    // for (i = 0; i < processCount; i++)
    // {
    //     resume(create(decrementValue_polling, 1024, 20, (char)i, 1, &index));
    // }
    // while (index < (processCount * rounds));
    /*End of polling Block*/

    /*Semaphores - Working */
    // if (processCount > 0 && processCount < 64 && rounds > 0)
    // {
    //     for (i = 0; i < processCount; i++)
    //     {
    //         if (i == 0)
    //         {
    //             process_semaphores[i] = semcreate(1);
    //         }
    //         else
    //         {
    //             process_semaphores[i] = semcreate(0);
    //         }
    //         process_doneSemaphores[i] = semcreate(0);
    //     }
    //     processValue_semaphore = (processCount * rounds) - 1;
    // }

    // for (i = 0; i < processCount; i++)
    // {
    //     resume(create(decrementValue_semaphore, 1024, 20, (char)i, 1, i));
    // }

    // for (i = 0; i < processCount; i++)
    // {
    //     wait(process_doneSemaphores[i]);
    //     semdelete(process_semaphores[i]);
    //     semdelete(process_doneSemaphores[i]);
    // }
    /*End of Semaphore Block*/

    return SHELL_OK;
}
