# Description

# Prerequisites
* You have followed the instructions in [[Repository Setup | Repository-Setup]]
* To build the xinu kernel, you will need to unset the following environment variable. You should add this line to your .bashrc. If you do not unset this variable, the build will segmentation fault on the SOIC linux machines which includes silo. 

```bash
unset MALLOC_PERTURB_
```
* Make sure that you have /l/arm2/devtools/bin in your path. You can add the following line to your .bashrc file to set the path.

```bash
export PATH=/l/arm2/devtools/bin:${PATH}
```

# Resources
* [Keys to control qemu, especially exit ...](http://wiki.qemu.org/download/qemu-doc.html#mux_005fkeys)

# Part 1 
The aim of this project is to get familiar with the Xinu
operating system structure and with modifying and compiling the
code. At the end of this project, you will be able to boot qemu
using the compiled Xinu image file that you create.

In this project we will be modifying the Xinu shell, which currently
supports a limited set of commands. Your job is to add a simple hello
command which will take a single string argument. If there are more
than or less than one string argument it should throw appropriate
error like too few or too many arguments.

```
Syntax: 
	xsh$ hello <string>
Output 
	Hello <string>, Welcome to the world of Xinu!!
```

## Steps to follow
* Navigate to the shell folder inside Xinu source code.
* Create a xsh_hello.c. 
* Refer to other command files’ contents to check standard command file structure. eg. xsh_date.c
* Write code for hello command in the xsh_hello.c file.
* Go to shell.c file inside the shell directory.
* Modify the commandtab array to support the newly added hello command. eg.
	{"hello", FALSE, xsh_hello}
* Go to include/shprototypes.h and add the function prototype of hello command. It should look like
	shellcmd xsh_hello(int, char *[]);
* Enter the directory **compile/uboot-tool** directory. Run *"make"*. You will only need to do this just this once for your repository. Typically when you build Xinu the mkimage tool will not need to be rebuilt.
* Enter the directory **compile**. 
* Compile the code by typing *"make"*. 
* Run the code using *"make qemu"*. Qemu should boot to a prompt. You can kill qemu using **CTRL-A X**. 
* Test your program with various inputs, including the correct, too many and too few arguments.

# Part 2
This needs to be done directly on linux on silo.

* Create a directory in your repository called **programs** at the root of your repository. 
* Inside of the **programs** directory create a directory named **assignment_1**.
* Create a C program in the **assignment_1** directory named
  sys_info.c. This program should have a Makefile that the program to
  be built by calling **make**.
* In this program, create 2 processes using fork() system call.
* Each process should print it's process id and whether it is the parent or child process.
* Parent process will send a string argument to child process using pipe()(write end of pipe). 
  The string written to the pipe is the argument given to the program on the command line. 
  Ex: date, uname, hostname, echo.
* Child process will receive the string argument from parent process through read end of the pipe.
* Child process should use execl() to run the program for the corresponding argument sent by parent process.

Syntax:
 ./sys_info date
```
Sample Output:
 Parent PID = x
 Child PID = y
 Thu Jan 28 21:32:36 EST 2016
For echo, child process should send "Hello World!" as an argument in exec().
```

# Part 3
Create a file Report_1.txt and answer the following:
* What is Orphan and Zombie processes?
* What are the changes that occur when the child process calls the exec system call to execute a new program?

Your report should be placed in a **reports** subdirectory that you create at the root of your repository. 

## What to turn in

* Your repository should contain your solutions to the parts above.
* Submit a link to the commit including your solutions the assignment in canvas. 
