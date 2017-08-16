# High Level Requirements
These aren't exhaustive, but should give you a general idea of what you are working on.

### Concepts
* Learn about the `create` system call.
* Create shell commands which can handle a variety of arguments in any order, and assign sane defaults.
* Continue adjusting to Xinu's file structure and working with Xinu.

### Tasks
* Create a new shell command, `process_ring` with a combination of the following arguments.
  * The number of processes `--processes` or `-p`.
  * The number of rounds `--rounds` or `-r`.
  * The version `--version` or `-v`.
  * Useful help text `--help` or `-h`.
* Create a report file with your report.
* Answer the questions in the lab.

# A Word About Xinu

Xinu was designed as an educational tool.  When it was being written certain decisions were made to allow students to easily understand and work with the kernel.  One such decision was that implementing virtual memory for Xinu would be left as an exercise to the reader of the textbook.  As a result, **Xinu _processes_ all share the same address space** (and can thus share variables directly), which is in stark contrast to the abstract concept of _processes_ that we have been discussing in class.  As things currently stand in the Xinu kernel, a Xinu process behaves much like the concept of a _thread_ that we have talked about.  In fact, you will notice that the `create` system call behaves more like `pthread_create` than it does `fork`, and that Xinu does not actually have a `fork` system call.

# Description

In this assignment you will learn about Xinu's *create* system call (information on this call can be found on page 111 of the book) and how to share data between processes.  The goal of this assignment is to create a Xinu program where a number of processes coordinate to count down from a given starting number.

> The goal of this assignment is to create a Xinu program where a number of processes coordinate to count down from a given starting number.

You will start by creating a new shell command called `process_ring`.  This program will create a number of process which count down an integer down to zero in rounds.  If there are 4 processes and 5 rounds, then the integer value being counted down is 20 (4 \* 5).

The following is example output from a program that is behaving correctly:

```
xsh$ process_ring
Number of Processes: 4
Number of Rounds: 5
Ring Element 0 : Round 0 : Value : 20
Ring Element 1 : Round 0 : Value : 19
Ring Element 2 : Round 0 : Value : 18
Ring Element 3 : Round 0 : Value : 17
Ring Element 0 : Round 1 : Value : 16
Ring Element 1 : Round 1 : Value : 15
Ring Element 2 : Round 1 : Value : 14
Ring Element 3 : Round 1 : Value : 13
Ring Element 0 : Round 2 : Value : 12
Ring Element 1 : Round 2 : Value : 11
Ring Element 2 : Round 2 : Value : 10
Ring Element 3 : Round 2 : Value : 9
Ring Element 0 : Round 3 : Value : 8
Ring Element 1 : Round 3 : Value : 7
Ring Element 2 : Round 3 : Value : 6
Ring Element 3 : Round 3 : Value : 5
Ring Element 0 : Round 4 : Value : 4
Ring Element 1 : Round 4 : Value : 3
Ring Element 2 : Round 4 : Value : 2
Ring Element 3 : Round 4 : Value : 1
ZERO!
```

**The program will create `--processes` or `-p` number of processes (with a default of 4)** and an array containing an integer for each.  These integers will represent a process's _inbox_ and will be how they communicate with each other.  **The program will also take a number of rounds `--rounds` or `-r`, with a default of 5**.  Once you have created each of the processes and informed them of their IDs (ranging from 0 to _NUM_PROCESSES_ - 1, inclusive), start the countdown by writing the value `(NUM_PROCESSES * num_rounds)` to the inbox of process 0.  Each process should then read the value, print out a message, decrement the value, and store it in the inbox of the next process in the ring (e.g. 0->1, 1->2, 2->3, 3->0).

A correctly behaving program can be very similar to a misbehaving program.  Small differences can drastically change what the program does.  For this homework you will need to create four versions of the above program that exhibit four different behaviors.  **The version behaviour of the program is specified using `--version` or `-v` (with the default behaviour being to work correctly)**.  The versions should do the following:

1. `-v work` Work correctly (default behaviour)
2. `-v hang` Hang
3. `-v loop` Go into an infinite loop
4. `-v chaos` Print numbers out of order

You will need to demonstrate each of these cases during the grading meeting.  Again, this is a *Xinu* command you are adding.  Do not turn in a program that uses _pthreads_ and runs on your Linux machine.

> Again, this is a *Xinu* command you are adding, not a linux command to run on Silo.

The following is an incomplete list of commands your program should handle, just in case you were thinking about being sloppy handling arguments:

```
process_ring

process_ring -p 3 -r 4
process_ring -v work --processes 3 -r 400
process_ring -p 3 -v work -r 4
process_ring --round 4 --version work -p 3

process_ring -v hang
process_ring -v loop
process_ring --version loop -p 6 -r 10
process_ring -v chaos

process_ring -h
process_ring --help

// These might take a while...
process_ring -p 1 -r 2147483647
process_ring --rounds 214748364 --processes 10
```

And here are some commands that your program should either go above and beyond to implement, or fail gracefully for (preferably with a nice error message):
```
// Think about why these shouldn't work.
// Some of these you could definitely make work if you wanted to,
// others would be very hard...
process_ring -p 243
process_ring -p 30 --rounds 214748364
process_ring -p 1 -r 21474836470

process_ring --version obama
process_ring -v trump

process_ring -V work
process_ring --VeRsIoN work

process_ring --random argument
process_ring --processes type_issues
process_ring -version 123

process_ring -p30
process_ring -pr 20
process_ring --vers work

process_ring chaos
process_ring handling edge cases is hard
```

We are going to be making quite a few shell commands, so now is probably a good time to think about how to handle arguments in a maintainable way.  We will be trying to break your commands, and we will take off points if we manage to.

Also think about adding arguments specific to your testing or debugging.  If you have specific features that aren't working, it might be good to add a `--test` option that runs through test cases, rather than typing in the same set of commands over and over.

# About the create System Call

The Xinu `create` system call can be used as follows:

```
resume( create(my_function, 1024, 20, "my function", 1, 42));
```

Here we create a process that will start at the function `my_function`.  It will be allocated a stack of 1024 bytes, given a priority of 20 (the highest priority level in Xinu, while 19 is the lowest priority in Linux), and assigned the name "my function".  The second to last argument tells the `create` function how many arguments `my_function` takes.  It is assumed that these arguments are of type `int32`.  Any arguments after this are passed on to `my_function`.

# Files to Create

* *include/process_ring.h* - This file will declare function prototypes and variables used in your program.
* *shell/xsh_process_ring.c* - This file will contain the code for your shell command.  It should create all of the ring processes.
* *apps/process_ring.c* - You should put the function that defines your ring processes here.

# Deliverables

**When you have your four working versions, create a *Report.md* file in the root of your "assignment2" branch**.  In this file write a brief description of how your program works and the differences between the four versions.  Copy and clearly label example output for each version.  Next, create a new section and answer each of the following questions.

> Clearly indicate in your report what work was done by each of the team members.

# Questions

1. Why does your program hang?  What is the name of this behavior in an operating system?
2. Why does your program go into an infinite loop?  What is the name of this behavior in an operating system?
3. Why does your program print numbers out of order?  What is the name of this behavior in an operating system?
4. What property of Xinu allows the working version of your program to print values in the correct order?

# Resources

* [_extern_ keyword](http://www.geeksforgeeks.org/understanding-extern-keyword-in-c/)
* [_volatile_ keyword](http://www.geeksforgeeks.org/understanding-volatile-qualifier-in-c/)