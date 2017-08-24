/*
 * @author Arun Nekkalapudi 
 */

 #include <stdio.h>
 #include <sys/types.h>
 #include <unistd.h>
 
 void main (int nargs, char *args[])
 {
	char *message[2];
	pid_t  pid1,pid2;
	  	
	pid1 = fork();
	pid2 = fork()

	flag=pipe(message);
	if(flag == 0){
		printf("pipe has been successfully created.\n");
	}
	else{
		printf("error in creating a pipe.\n")
	}


	/*for Process ID 1*/
	if (pid1 == 0){ 
		printf("Child PID 1: %d\n",getpid());
		if((strlen(args[1])>0) && (args[1] != NULL) && (nargs > 1){
			execl("/bin/echo","echo","list ","of", "files:\n",NULL);
		}
	}		   
	else{ 
		printf("Parent PID 1: %d\n",getpid());	  
	}

    /*for Process ID 2*/
	if (pid2 == 0){ 
        printf("Child PID 2: %d\n",getpid());
	}		   
	else{ 
		printf("Parent PID 2: %d\n",getpid());	  
	}

	
 }
