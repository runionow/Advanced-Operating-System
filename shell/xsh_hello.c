/* xsh_hello.c - xsh_hello */

/*
 * @author Arun Nekkalapudi
 */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * hello <string> - Hello <string>, Welcome to the world of Xinu!!
 *------------------------------------------------------------------------
 */
 
shellcmd xsh_hello(int nargs, char *args[]) {

    /* Print Hello-World*/
    
    if ((strlen(args[1]) > 0) && (args[1] != NULL) && (nargs > 1)) {
	if (nargs > 2){
        	printf("Unable to get your name too many i/p arguments.\n");
    	}
	else {
		printf("Hello %s, Welcome to the world of Xinu!!\n", args[1]);
    	}
    }
   else {
        printf("Missing i/p arguments.\n");
   }

   return 0;
}
