/*
 * @author Arun Nekkalapudi 
 */

 #include  <stdio.h>
 #include  <sys/types.h>

 void  main(void)
 {
	  pid_t  pid;
 
	  pid = fork();
	  if (pid == 0) 
		   ChildProcess();
	  else 
		   ParentProcess();
 }
 
 void  ChildProcess(void)
 {
	  printf("For Child process execute this function ***\n");
	  printf("Child Process ID: %d",&pid);
 }
 
 void  ParentProcess(void)
 {
	  printf("For Parent Process execute this function\n");
	  printf("Parent Process ID : %d",&pid);
	  
 }
