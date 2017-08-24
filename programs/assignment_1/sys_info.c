/*
 * @author Arun Nekkalapudi 
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>


void main(void){
	pid_t procID;
	procID = fork();
	printf("%d\n",&procID);
    	printf("Hello this is arun\n");
}
