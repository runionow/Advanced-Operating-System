/*
 * @author Arun Nekkalapudi 
 */

 #include  <stdio.h>
 #include  <sys/types.h>
 
 void ChildProcess(void);
 void ParentProcess(void);
 
 void  main(void)
 {
	  pid_t  pid;
 
	  pid = fork();
	  if (pid == 0){ 
		   ChildProcess();
                   printf("Child Process ID : %d\n",getpid());
	  }		   
	  else{ 
		   ParentProcess();
	           printf("Parent Process ID : %d\n",getpid());	  }
 }
 
 void  ChildProcess(void)
 {
	  printf("For Child process execute this function ***\n");
 }
 
 void  ParentProcess(void)
 {
	  printf("For Parent Process execute this function\n");	  
 }
