/* xsh_arg_example.c - xsh_arg_example */

/*------------------------------------------------------------------------
 * xhs_arg_example - display the value associated with the -p flag
 *------------------------------------------------------------------------
 */

#include <xinu.h>
#include <stdlib.h>
#include <stdio.h>

void print_usage(void){
  printf("Usage: ...\n");
}

shellcmd xsh_arg_example(int argc, char *args[])
{
  int32 p = 2; /* specified default */
  bool p_given = FALSE;
  char *endp;
  int32 i;
  for(i = 1; i < argc; i++){
    if (0 == strncmp("-p", args[i], 3)){
      if (! (i + 1 < argc)) {
        print_usage();
        printf("-p flag expected an argument\n");
        return SHELL_ERROR;
      }
      if (p_given) {
        printf("-p flag expected at most once\n");
        return SHELL_ERROR;
      }
      /* parse numeric argument to -p */
      p = strtol(args[i + 1], &endp, 10);
      if (endp == args[i + 1]) {
        /* The end pointer hasn't advanced */
        print_usage();
        printf("-p flag expected an integer\n");
        return SHELL_ERROR;
      } else if (*endp != '\0'){
       /* There was trailing garbage in the string that
          wasn't converted to an integer. */
        print_usage();
        printf("-p recieved invalid integer\n");
        return SHELL_ERROR;
      } else if (!(0 <= p && p <= 64)) {
        /* The number is out of range */
        print_usage();
        printf("-p flag expected a number between 0 - 64\n");
        return SHELL_ERROR;
      }
      p_given = TRUE;
      i += 1; /* Skip pass the numeric argument */
    } else /* if (...) */ { 
      // Handle other cases
    }
  }
  printf("argument  is %d\n", p);
  return SHELL_OK;
}
