/* xsh_argecho.c - xsh_argecho */

#include <xinu.h>
#include <stdlib.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xhs_argecho - display argecho message that lists shell commands
 *------------------------------------------------------------------------
 */
void print_usage(void){
  printf("Usage\n");
}

shellcmd xsh_arg_example(int argc, char *args[])
{
  int32 p = 2; /* specified default */
  char *endp;
  int32 i;
  for(i = 1; i < argc; i++){
    if (0 == strncmp("-p", args[i], 3)){
      /* parse numeric argument to -p */
      if (! (i + 1 < argc)) {
        print_usage();
        printf("-p flag expected an argument\n");
        return SHELL_ERROR;
      }
      p = strtol(args[i + 1], &endp, 10);
      if (endp == args[i + 1]) {
        /* The end pointer hasn't advanced */
        print_usage();
        printf("-p flag expected an integer\n");
        return SHELL_ERROR;
      } else if (!(0 <= p && p <= 64)) {
        /* The number is out of range */
        print_usage();
        printf("-p flag expected a number between 0 - 64\n");
        return SHELL_ERROR;
      }
      i += 1; /* Skip pass the numeric argument */
    } else /* if (...) */ { 
      // Handle other cases
    }
  }
  printf("argument  is %d\n", p);
  return SHELL_OK;
}
