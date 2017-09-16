/* newqueue.c - newqueue */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  newqueue  -  Allocate and initialize a queue in the global queue table
 *------------------------------------------------------------------------
 */
qid16	newqueue(void)
{
	static qid16	nextqid=NPROC;	/* Next list in queuetab to use	*/
	qid16		q;		/* ID of allocated queue 	*/

	q = nextqid;
	if (q > NQENT) {		/* Check for table overflow	*/
		return SYSERR;
	}

	nextqid += 2;			/* Increment index for next call*/

	/* Initialize head and tail nodes to form an empty queue */

	queuetab[queuehead(q)].qnext = &queuetab[queuetail(q)];
	queuetab[queuehead(q)].qprev = NULL;
	queuetab[queuehead(q)].qkey  = MAXKEY;
	queuetab[queuehead(q)].pid  = q;
	queuetab[queuetail(q)].qnext = NULL;
	queuetab[queuetail(q)].qprev = &queuetab[queuehead(q)];
	queuetab[queuetail(q)].qkey  = MINKEY;
	queuetab[queuetail(q)].pid  = q+1;

	return q;
}
