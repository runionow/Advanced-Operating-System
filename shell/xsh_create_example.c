/* xsh_create_example.c - xsh_create_example */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_create_example - print a familiar and friendly greeting
 *------------------------------------------------------------------------
 */

/* print x on n lines */
process print_it(int32 x, int32 n){                                                  
  int i;
  for (i = 0; i < n; i++){                                                          
    printf("%d\n", x);
  }
  return OK;
}  
 
shellcmd xsh_create_example(int nargs, char *args[]) {
  /* This process has a lower priority than the parent process */
  resume(create(print_it, 1024, 1 , "after",  2, 21,  3));
  /* This process has a higher priority than the parent process */
  resume(create(print_it, 1024, 30, "before", 2, 42, 10));
  /* I should use syncronization to keep the parent from exiting */
  return SHELL_OK;
}
