/* queue.c - enqueue, dequeue */

#include <xinu.h>

struct qentry	queuetab[NQENT];	/* Table of process queues	*/

/*------------------------------------------------------------------------
 *  enqueue  -  Insert a process at the tail of a queue
 *------------------------------------------------------------------------
 */
pid32	enqueue(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q		/* ID of queue to use		*/
	)
{
	struct qentry *tail;
        struct qentry *prev;		/* Tail & previous Structs */

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	tail = &queuetab[queuetail(q)];
	prev = queuetab[tail->pid].qprev;
        /*Insert just before node*/
	queuetab[tail->pid].qnext  = &queuetab[pid];
	queuetab[pid].qnext  = &queuetab[tail->pid];
	queuetab[pid].qprev = prev;
	prev->qnext = &queuetab[pid];
	return pid;
}

/*------------------------------------------------------------------------
 *  dequeue  -  Remove and return the first process on a list
 *------------------------------------------------------------------------
 */
pid32	dequeue(
	  qid16		q		/* ID queue to use		*/
	)
{
	pid32	pid;			/* ID of process removed	*/

	if (isbadqid(q)) {
		return SYSERR;
	} else if (isempty(q)) {
		return EMPTY;
	}

	pid = getfirst(q);
	queuetab[pid].qprev = NULL;
	queuetab[pid].qnext = NULL;
	return pid;
}
