/* xsh_process_ring.c - xsh_polling_example */

/*
 * @author Arun Nekkalapudi
 */ 

 #include <xinu.h>
 #include <string.h>
 #include <stdio.h>
 
 /*------------------------------------------------------------------------
  * Process_Ring-Example
  *------------------------------------------------------------------------
  */
 
 /*Initializing queue buffer with 1*/ 
 int32 buffer = 1; 

  
 char* concat(const char *string1, const char *string2)
 {
     char *result = malloc(strlen(string1)+strlen(string2)+1);
     strcpy(result, string1);
     strcat(result, string2);
     return result;
 }


 process process_ring(int32 *produce, volatile int32 *consume,int32 processNumber){
    int32 last = -1;
    while (last < 3){
      /* Polling to see if the consumer has consumed the last value */
      int32 tmp = *consume;
      if (last == tmp){
        printf("Ring Element %d RoundP: %d\n", processNumber,tmp+1);
        last = tmp + 1;
        *produce = tmp + 1;
      }
      else if(last != temp){
        printf("Ring Element %d RoundC: %d Value: \n", processNumber,tmp);
        last = tmp;
        *consume = tmp;
      }                                                                           
    }                                                                               
    return OK;   
}


 shellcmd xsh_process_ring(int nargs, char *args[]) {
 
     /* Create_example*/
     int32 produce = -1;
     volatile int32 consume = -1;
     char concatString[] = "Process";
    
         /*Update Value 2 here [Hard coded]*/     
        for(i=0;i<2;i++){
            strcat()
            resume(create(process_ring,1024,20,concat(concatString, (char) i),3,&produce,&consume,i));     
        }
    
     
     /*I should keep the synchronization to prevent the parent from exiting*/
     
     /*Update Value 2 here [Hard coded]*/
     while (consume<3);
    
     return SHELL_OK;
 }
 
