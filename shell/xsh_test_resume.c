#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

struct procent *prptr;

void resume_test(int x);

shellcmd xsh_test_resume(int nargs, char *args[]) {
  pri16 prior;  /*Initiliaztion of Priorities*/
  pid32 proc_new = create(resume_test,1024,20,"f",1,1); /*Creating a Process with processID 20*/
  prior = resume(proc_new); /*returns the priority*/
  //sleep(4);
  printf("Priority :%d\n",prior);   /*Here we can observer the changes in both Scenario 1 and Scenario 2*/
  return 0;
}

void resume_test(int x){
	prptr = &proctab[getpid()]; /*Getting the proctab address using pid of the current process*/
	prptr->prprio = 40; /*Modifying the priority.*/
}
