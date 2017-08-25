#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

/*
* FUNCTION : CONCAT
* DESCRIPTION : To Concatenate two Strings.
* ARGUMENTS : char *string1 , char *string2.
* RETURNS : Concatenated String.
*/

char* concat(const char *string1, const char *string2)
{
    char *result = malloc(strlen(string1)+strlen(string2)+1);
    strcpy(result, string1);
    strcat(result, string2);
    return result;
}

int main(int nargs,char *argv[])
{	
        int message[2], rcv_msg;
        pid_t childpid;
        char temp_message[] = "Hello World!";
        char concatString[] = "/bin/";
        char storageBuffer[80];
        
        /* PIPE:
         * message[0] -> Read.
         * message[1] -> Write.
         */
        pipe(message);
        
        /*Execute fork()*/
        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }
        
        /*Check for PARENT*/
        if(childpid != 0)
        {
				/*PARENT*/
				printf("Parent PID = %d\n",getpid());
                close(message[0]);
				
				/*Writing argument to Pipe*/
                if(nargs == 2){
                    write(message[1], argv[1], (strlen(argv[1])+1));
                }
                else{
					if(nargs > 2){
						printf("More than one argument has been passed to the program.\n");
					}
					else if(nargs == 1){
						printf("No command line arguments has been passed to the program.\n")
					}
                    write(message[1], temp_message, (strlen(temp_message)+1));
                }
                
                exit(0);
        }
        else if(childpid == 0)
        {
				/*CHILD*/
                printf("Child PID = %d\n",getpid());
				close(message[1]);
				
				/*Reading from Pipe*/
				rcv_msg = read(message[0], storageBuffer, sizeof(storageBuffer));
				
				/*Concating strings to generate bash path. Eg: /bin/date */
				char *concatString1 = concat(concatString, storageBuffer);
				
				/*Handled echo*/
                if((!(strcmp(storageBuffer,temp_message) == 0)) && (!(strcmp(storageBuffer,"echo") == 0)) ){
                    printf("Received string [CHILD]: %s\n", storageBuffer);
                    execl(concatString1,storageBuffer, 0);
				}
				else if(strcmp(storageBuffer,"echo") == 0){
					/*Running Bash-echo Commands using execl*/
                    execl(concatString1,storageBuffer,temp_message, 0);
				}
                else {
                    printf("Received String [CHILD]: %s\n", storageBuffer);
                }
        }
        else{
            printf("Failed to create a Process.");
		}
		
        return(0);
}
