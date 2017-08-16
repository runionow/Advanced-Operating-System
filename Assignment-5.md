# Description

The aim of this assignment is to understand and implement futures in the Xinu operating system. In this project you will be responsible for developing and testing the system calls required for futures.

## Futures

Futures are a mechanism for synchronizing on asynchronously produced data, potentially improving the ability to manage concurrent activities efficiently. Please refer to following links for more information:

1. [[http://en.wikipedia.org/wiki/Futures_and_promises]]
2. [[http://docs.scala-lang.org/sips/completed/futures-promises.html]]

# Implementation

Use the following code and details to implement futures in the Xinu operating system.

## Data Structures and Types
```C
typedef enum {
  FUTURE_EMPTY,
  FUTURE_WAITING,
  FUTURE_READY
} future_state_t;

typedef enum {
  FUTURE_EXCLUSIVE,
  FUTURE_SHARED,
  FUTURE_QUEUE
} future_mode_t;

typedef struct {
  int value;
  future_state_t state;
  future_mode_t mode;
  pid32 pid;
  yourtype set_queue;
  yourtype get_queue;
} future_t;
```

### Field Descriptions

* **value** - The value which will be held in future.

* **mode** - Mode of operation for future.  It must be defined while allocating future using `future_alloc()` For this assignment we will support `FUTURE_EXCLUSIVE`, `FUTURE_QUEUE` and `FUTURE_SHARED` modes.

  * `FUTURE_EXCLUSIVE` - In this mode maximum 2 threads can get involved. One thread will set the value of future and other will get the value of future.
 *  `FUTURE_SHARED` - In this mode there is one-to-many relationship between thread calling `future_set()` and `future_get()`. Only one thread can call `future_set()`. Any subsequent calls to `future_set()` function should throw an error. Many threads can call `future_get()` function on a future and get the value. All threads waiting on a future for value should be queued in `get_queue` of the future. As soon as a future is
set, all threads waiting in `get_queue` should be resumed. When you call`future_free()` function to free the future, it will also free the `get_queue` and destroy it.
  * `FUTURE_QUEUE` - In this mode threads calling `future_set()` and `future_get()` share many-to-many  relationship. The future implementation will include `set_queue` and `get_queue` to hold the information about the waiting threads.You should implement your own queue type. Many threads can call `future_set()` and `future_get()`.  A future in this mode should behave as follows:
    * If a thread is calling `future_set()` and there are threads waiting for a value in the `get_queue` then the thread calling `future_set()` should set the value and resume only one thread from the `get_queue` based on first come first serve basis. If there is no thread waiting in the `get_queue` then thread calling `future_set()` should enqueue itself in the `set_queue`.
    * If a thread is calling `future_get()` and there are threads waiting to set value in the `set_queue` then thread should release only one thread from the `set_queue` and should get the value set by thread just released from `set_queue`.  If there is no thread waiting in `set_queue` to set the value then thread calling `future_get()` should enqueue itself in the `get_queue`.

* **state** - There are 3 valid states for future to be in:
  1. `FUTURE_EMPTY` – A future allocated for an operation by a call to `future_alloc()`
  2. `FUTURE_WAITING` – A future on which a process is waiting, by a call to `future_get()`, for a value to be assigned
  3. `FUTURE_VALID` – A future that holds a value assigned by a call to `future_set()`

* **pid** - Process id of thread waiting for value by calling `future_get()`

## The Futures Interface

* `future_t* future_alloc(future_mode_t mode)` - Allocates a new future (in the `FUTURE_EMPTY` state) with the given mode. We will use the `getmem()` call to allocate space to new future.
  * Parameters:
    * `future_mode_t mode` - The mode this future will operate in
  * Returns: `future_t` - NULL or a pointer to a valid future
* `syscall future_free(future_t* f)` - Frees the allocated future. Use the `freemem()` system call to free the space.
  * Parameters:
    * `future_t* f` - future to free
  * Returns: `syscall` - SYSERR or OK
* `syscall future_get(future_t* f,  int* value)` - Get the value of a future set by an operation and change the state of future from `FUTURE_VALID` to `FUTURE_EMPTY`.
  * Parameters:
    * `future_t* f` - Future on which to wait for and obtain value
    * `int* value` - Address into which the future's value will be copied.
  * Returns: `syscall` - SYSERR or OK
* `syscall future_set(future_t* f, int value)` - Set a value in a future and changes state from  `FUTURE_EMPTY` or `FUTURE_WAITING` to `FUTURE_VALID`.
  * Parameters:
    * `future_t* f` - Future in which to set the value
    * `int value` - Result of an operation to be set as value in the future
  + Returns: `syscall` - SYSERR or OK

# Instructions
1. Copy the `future.h` code listing below into the `include/future.h` file.
2. Implement the required functions in the `system/future.c` file.
3. Modify `shell/xsh_prodcons.c` to accept a `-f` flag.  When this flag is given the program should use the code listed in **Future ProdCons Snippet** bellow.
4. Create the file `apps/future_prodcons.c` and add the `future_prod` and `future_cons` functions listed bellow.

# Code Listings

## `future.h`
```C
#ifndef _FUTURE_H_
#define _FUTURE_H_  

typedef enum {
  FUTURE_EMPTY,
  FUTURE_WAITING,
  FUTURE_READY
} future_state_t;

typedef enum {
  FUTURE_EXCLUSIVE,
  FUTURE_SHARED,
  FUTURE_QUEUE
} future_mode_t;

typedef struct {
  int value;
  future_state_t state;
  future_mode_t mode;
  pid32 pid;
  yourtype set_queue;
  yourtype get_queue;
} future_t;

/* Interface for the Futures system calls */
future_t* future_alloc(future_mode_t mode);
syscall future_free(future_t*);
syscall future_get(future_t*, int*);
syscall future_set(future_t*, int);
 
#endif /* _FUTURE_H_ */
```

## Future ProdCons Snippet

```C  
  future_t* f_exclusive,
          * f_shared,
          * f_queue;
 
  f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
  f_shared    = future_alloc(FUTURE_SHARED);
  f_queue     = future_alloc(FUTURE_QUEUE);
 
  // Test FUTURE_EXCLUSIVE
  resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
  resume( create(future_prod, 1024, 20, "fprod1", 1, f_exclusive) );

  // Test FUTURE_SHARED
  resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
  resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
  resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) ); 
  resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
  resume( create(future_prod, 1024, 20, "fprod2", 1, f_shared) );

  // Test FUTURE_QUEUE
  resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
  resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
  resume( create(future_cons, 1024, 20, "fcons8", 1, f_queue) );
  resume( create(future_cons, 1024, 20, "fcons9", 1, f_queue) );
  resume( create(future_prod, 1024, 20, "fprod3", 1, f_queue) );
  resume( create(future_prod, 1024, 20, "fprod4", 1, f_queue) )
  resume( create(future_prod, 1024, 20, "fprod5", 1, f_queue) );
  resume( create(future_prod, 1024, 20, "fprod6", 1, f_queue) );
```

# `future_prodcons.c`
```C
int n = 0;
uint future_prod(future_t* fut) {
  int i;
  n++;
  i = n;
  n++;//just to make every producer produces different numbers
  printf("Produced %d\n",i);
  future_set(fut, i);
  return OK;
}

uint future_cons(future_t* fut) {
  int i, status;
  status = (int)future_get(fut, &i);
  if (status < 1) {
    printf("future_get failed\n");
    return -1;
  }
  printf("Consumed %d\n", i);
  return OK;
}
```

# How to Submit Your Assignment

You need to submit assignment on Canvas. It will include GitHub commit hash-code, time stamp and description. Your submission will also contain report file which will include the functionality developed by each group member.
