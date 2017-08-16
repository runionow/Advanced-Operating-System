# High Level Requirements
These aren't exhaustive, but should give you a general idea of what you are working on.

### Concepts
* Learn about mutexes and condition variables.
* Create new data types and system calls in Xinu.

### Tasks
* Create a new shell command, `babybird` with a combination of the following arguments.
  * The number of baby birds `-b` or `--babies`.
  * The number of worms the mother fetches `-f` or `--fetch`.
  * The number of worms each baby needs to eat `-e` or `--eat`.  Any less and they die of starvation, any more and they die of overeating.  Don't kill the birds please.
  * Useful help text `--help` or `-h`.
* Create a report file with your report.
* Answer the questions in the lab.

# Description
The aim of this assignment is to understand how _mutexes_ and _condition variables_ are implemented and how they may be used to synchronize access to shared resources.  You will need to create several Xinu system calls to add support for these mechanisms.

To demonstrate your understanding of the use of mutexes and condition variables you will write an implementation of what we'll call the _baby bird_ problem.  You will start by creating a new shell command called `babybird` that takes three numerical arguments, `num_baby_birds`, `num_fetch_worms`, and `num_eat_worms`.  In the program a number of baby birds will take turns eating worms out of a common dish (the shared resource).  Only one bird may eat from the dish at a time, and each bird may only eat one worm at a time before allowing another bird to eat (it's important to teach children fairness).  If one of the baby birds find the dish empty then they must use a _condition variable_ to tell the parent bird that more worms are needed.  When this happens the parent bird will wake up and put `num_fetch_worms` into the dish before allowing the baby birds to eat again.

This program must print out the ID of the baby bird when it eats a worm, as well as the number consumed by the bird thus far (e.g. `Baby bird 1 ate a worm! (3 total)`), and the parent bird must print out when it fills the dish up (e.g. `Parent bird filled the dish with 10 worms!`).  The program must exit cleanly after each bird has eaten *EXACTLY* it's share of the worms (`num_eat_worms`).  When the program exits print out the number of worms left in the dish.  This will help us ensure that none of your baby birds has starved to death.

*DO NOT USE THE SLEEP SYSTEM CALL*.  The baby birds are hungry and want to eat worms as fast as they can.

Example program invocations:

```
xsh$ babybird -b 4 -e 5 -f 6
xsh$ babybird --eat 5 --babies 4 -f 6
xsh$ babybird --fetch 10 --eat 42 --babies 12

xsh$ babybird -h
xsh$ babybird --help
```

Examples where your program should fail gracefully:

```
xsh$ babybird -b 4 -f 6
xsh$ babybird -b 4 --babies 5 -f 6
xsh$ babybird 4 5 6
xsh$ babybird --foo bar --babies 5 -f 6 --eat 12
xsh$ babybird --babies four -e 5 -f 6
```

# Implementation Details

You will need to create two new data types (by either using `typedef` or defining a struct) and six new new system calls.  The new system calls are listed below and should live somewhere in the `system/` directory:

* `syscall mutex_create(mutex_t* lock)`: Create a mutex and set its initial state.
* `syscall mutex_lock(mutex_t* lock)`: Obtain the lock.  If the lock is already held the process must wait.  When the call returns the process *must hold the lock*.
* `syscall mutex_unlock(mutex_t* lock)`: Release the lock.
* `syscall cond_init(cond_t* cv)`: Create a condition variable and set its initial state.
* `syscall cond_wait(cont_t* cv, mutex_t* lock)`: Wait on the condition variable.  While the process is waiting the lock should be released so that other processes may obtain it.
* `syscall cond_signal(cont_t* cv)`: Wake up at least one of the processes waiting on the condition variable.

While the ARM processors in our BeagleBone Blacks do have atomic instructions (LDREX and STREX) you will not be writing assembly code.  Instead, you are expected to emulate *either* the Compare-And-Swap or Test-And-Set instructions via a helper function (which should also be implemented in a file in the `system/` directory).  You will then use this function to implement your mutex and condition variables.  This helper function is the *ONLY PLACE* you are allowed to disable interrupts.

Interrupts may be disabled as follows:

```C
void foo(void) {
  intmask mask = disable();
  // Critical Section
  restore(mask);
}
```

Your mutex and and condition variable implementations may either busy wait or alter the process table to suspend or resume the necessary processes.  There will probably be no difference with respect to grading, but you will get extra cool points with the AIs if you suspend and resume your processes.

# Deliverables
Create a Report.md file in the root of your "assignment3" branch. In this file write a brief description of how your program works and which lock mechanism you have implemented (e.g. Test-And-Set or Compare-And-Swap). Next, create a new section and answer each of the following questions.

# Questions
Some programmers designed the protocol shown below to achieve n-thread mutual exclusion. Answer the following questions:

1. What is starvation? Is this protocol starvation-free?
2. What is deadlock? Is this protocol deadlock-free?

```Java
class AOS_Test1 implements Lock {
  private int turn;
  private boolean busy = false;
  public void lock() {
    int me = ThreadID.get();
    do {
      do {
        turn = me;
      } while (busy);
        busy = true;
    } while (turn != me);
  }
  public void unlock() {
    busy = false;
  }
}
```
