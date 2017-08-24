/*
 * @author Arun Nekkalapudi 
 */

 #include <stdio.h>
 #include <sys/types.h>
 #include <unistd.h>
 #include <string.h>

 int main (int nargs, char *args[])
 {
	int message[2],rec_msg;
	char buffer[80];
	pid_t  pid1,pid2;
	char message1[] = "Hello, World \n";  	
	pid1 = fork();
	/*pid2 = fork();*/
        int flag;
	flag = pipe(message);
	if(flag == 0){
		printf("pipe has been successfully created.\n");
	}
	else{
		printf("error in creating a pipe.\n");
	}


	/*for Process ID 1*/
	if (pid1 == 0){
		close(message[1]);
		printf("Child PID 1: %d\n",getpid());
		rec_msg = read(message[0],buffer,sizeof(buffer));
		printf("Recieved Message: %s",rec_msg);
		printf("argumnents: %s",args);
		if((strlen(args[1])>0) && (args[1] != NULL) && (nargs > 1)){
			printf("Rambo");
			execl("/bin/echo","echo","Hello World");
			return 0;
		}
	}		   
	else{ 
		close(message[0]);
		printf("Parent PID 1: %d\n",getpid());
		write(message[1], message1, (strlen(message1)+1));		  
	}

    	/*for Process ID 2*/
	
	/*if (pid2 == 0){ 
        *printf("Child PID 2: %d\n",getpid());
	*}		   
	*else{ 
	*	printf("Parent PID 2: %d\n",getpid());	  
	}*/

	return 1;	
 }
