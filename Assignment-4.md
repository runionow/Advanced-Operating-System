## Objective

In this assignment, you will have an opportunity to use your understanding of xinu semaphores to solve the following classical synchronization problems.

* Producer-Consumer
* Reader-Writers 
* Dining Philosophers

## Overview

You will be implementing solutions to the Producer-Consumer (p55-65),
and Reader's Writers (p65-p79), Dining Philosopher's (p87-99) problems
demonstrated in the
[Little Book of Semaphores](http://greenteapress.com/wp/semaphores/). 

## Preliminary Steps

Create a function that can be called by your solution that will provide a random process delay in uS up to a programmable maximum that is specified as a an argument.

```c
void rand_delay(int uS_max);
```

## 

### Producer-Consumer

You should implement a solution to the **Finite Buffer
Producer-Consumer** problem outlined in section 4.1.4 on p61. The
buffer size will be parameterizable. You will also use your random
delay process to inject random delays into the production and
consumption of the data for the buffer. This random delay would be
used to model variable timing in the waitForEvent() (producer) process
and the event.process() (consumer) process. 

Your xinu command should have the following form.

```
producer-consumer <buffer size in bytes> <bytes to send> <max delay>
```

This command should create a producer process and a consumer process
with the associate buffer size. As the producer adds bytes to the
buffer, they should represent an increasing 8-bit unsigned
counter. The counter should reset at 0xff back to 0x00.

When the producer creates a byte, print the following:

```
Producer: <count value>
```

Similarly, when the consumer creates a byte, print the following:

```
Consumer: <buffer value> 
```

The producer should terminate when all of the bytes have been
sent. Similarly, the consumer should terminate when all of the bytes
have been received.

### Readers-Writers

You should implement a solution to the **Starvation Problem** outlined
in section 4.2.3 on p71. As with the Producer-Consumer problem, you
will model the **critical section** access timing with your
random_delay function.

Your xinu command should have the following form. 

```
readers-writers <num writers> <num readers> <write cycles> <read cycles> <max delay>
```

Your xinu command should create the number of writers and readers
specified. Then the number of write and read cycles should be
completed for each reader and writer.

The critical section for each Reader/Writers should do the following. 

* Print the following message

```
<Reader|Writer> <writer number> <cycle number>
```
* Delay a variable amount of time using rand_delay().

### Dining Philosophers

You should implement the solution to the **Dining Philsophers**
outlined in section 4.4.3 on p93. Again, you should model **think()**
and **eat()** using your random_delay function.

Your xinu command should have the following form.

```
dining-philosophers <num philosophers> <num cycles> <max delay>
```

You should create a process for each philosopher. Each philosopher
should have output that looks like the following:

```
Philosopher <philosopher number> <thinking|eating> <cycle number>
```

## What to turn in

Please submit a link to the assignment in canvas that includes your
solution. Additionally, you will be proving a demo and answering
questions about your solution in the discussion section.

## Scoring

* Code Solution 
  * rand_delay (10 pts) 
  * Producer-Consumer (20 pts)
  * Readers-Writers (20 pts)
  * Dining-Philosophers (20 pts)
* Demo/Questions (30 pts) 
