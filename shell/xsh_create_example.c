/* xsh_create_example.c - xsh_create_example */

/*
 * @author Arun Nekkalapudi
 */

 #include <xinu.h>
 #include <string.h>
 #include <stdio.h>
 
 /*------------------------------------------------------------------------
  * Create System Call
  *------------------------------------------------------------------------
  */
  

 process print_it(int32 x,int32 n){
     int i;
     printf("The value of x %d:\n",x);
     printf("The value of n %d:\n",n);     
     for (i = 0;i < n;i++){
        printf("%d\n",x);
     }
     return OK;
 }
 shellcmd xsh_create_example(int nargs, char *args[]) {
 
     /* Create_example*/
     /* This process has the lower priority than the parent process*/
     resume(create(print_it,1024,1,"after",2,21,3));

     /* This process has the higher priority than the parent process*/
     resume(create(print_it,1024,30,"after",2,42,10));
     
     /*I should keep the synchronization to prevent the parent from exiting*/
     return SHELL_OK;

    }
 