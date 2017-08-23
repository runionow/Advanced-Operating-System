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
    
	if (strlen(args[1]) > 0) {
		printf("Hello %s, Welcome to the world of Xinu!!", args[1]);
		return 0;
    }
    else if (nargs > 2){
        printf("Unable to get your name too many i/p arguments");
        return 0;
    }
    else if (nargs == 1){
        printf("Hello Anonymous, Welcome to the world of Xinu!!");
        return 1;
    }
}
