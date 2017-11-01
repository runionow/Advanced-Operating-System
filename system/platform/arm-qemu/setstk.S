/* start.S - start, bzero */

#include <armv7a.h>

#define NULLSTK 8192
/*------------------------------------------------------------------------
 * set stack pointer, R0 holds the new stack pointer.  
 *------------------------------------------------------------------------
 */
	.text
	.globl	setstk			

	.func setstk
setstk:
	mov sp, r0    
	bx lr
	.endfunc
