# Starting QEMU

The following Makefile argument will start QEMU with an exposed GDB
port. If you are running on a shared machine, you may need to change
the port number in the Makefile. You can do this my editing the
Makefile in the compile directory. The variable that you should change
is named **GPORT** at the top of the file. You should make this a port
number that is greater than 10000 and less than 65535.

You can use the linux checkport command to identify if this is an open port.

```bash
[bhimebau@silo:~] $ checkport 55600
Port 55600 is not currently being used.
[bhimebau@silo:~] $ 
```

Once you have determined an open port and adjusted the Makefile
accordingly, you can start qemu wih the exposed gdb port on the port that selected by running the make argument **make qemu-gdb**

```bash
[bhimebau@batman:~/forge/SOIC-Operating-Systems/P536-F16/compile] $ make qemu-gdb
Rebuilding the .defs file

Loading object files to produce xinu
Image Name:   
Created:      Fri Nov  4 09:49:19 2016
Image Type:   ARM Linux Kernel Image (uncompressed)
Data Size:    57344 Bytes = 56.00 kB = 0.05 MB
Load Address: 0x00010000
Entry Point:  0x00010000
export QEMU_AUDIO_DRV=wav; export QEMU_WAV_PATH=../compile/.wav.wav;qemu-system-arm -M versatilepb -m 512M -nographic -cpu arm1176 -kernel xinu.boot -S -gdb tcp::55600
```
# Starting GDB

**From a new terminal window**, you will now need to start
arm-none-eabi-gdb on the xinu elf file in the compile
directory. Notice that the file is named **xinu**. Also notice that
you need to use the architecture specific gdb rather than the native
x86 based gdb. Recall that we are using the ARM architecture for our
Xinu build.

```bash
[bhimebau@batman:/nfs/nfs6/home/bhimebau/forge/SOIC-Operating-Systems/P536-F16/compile] $ arm-none-eabi-gdb xinu
GNU gdb (GNU Tools for ARM Embedded Processors) 7.6.0.20140529-cvs
Copyright (C) 2013 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "--host=i686-linux-gnu --target=arm-none-eabi".
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>...
Reading symbols from /nfs/nfs6/home/bhimebau/forge/SOIC-Operating-Systems/P536-F16/compile/xinu...done.
(gdb) 
```

# Connecting to QEMU

Your GDB instance now has the symbols for the xinu build. However, it
is not attached to the running qemu instance. In this step you will
need to use the port number that you selected earlier.

```bash
(gdb) target extended-remote :55600
Remote debugging using :55600
0x00000000 in ?? ()
```
The load the code for the program into qemu.

```bash
(gdb) load
Loading section .text, size 0xd000 lma 0x10000
Loading section .data, size 0x1000 lma 0x1d000
Start address 0x10000, load size 57344
Transfer rate: 28000 KB/sec, 1911 bytes/write.
```

The qemu virtual machine is now waiting at the first instruction in the build.

# Running the xinu binary

Use the gdb instruction **continue** to start the build running. The
**run** instructure does not work when you are working remotely as we
are in this case.

```bash
(gdb) cont
Continuing.
```

On the terminal window where you started qemu, you should see the
familiar Xinu boot messages.

```bash
           [0x001D2000 to 0x1FFFDFFF]
     50096 bytes of Xinu code.
           [0x00010000 to 0x0001C3AF]
   1787136 bytes of data.
           [0x0001D000 to 0x001D14FF]


...creating a shell


------------------------------------------
   __    __   _____    _   _    _    _    
   \ \  / /  |__ __|  | \ | |  | |  | |   
    \ \/ /     | |    |  \| |  | |  | |   
    / /\ \    _| |_   | \   |  | |  | |   
   / /  \ \  |     |  | | \ |  \  --  /   
   --    --   -----    -   -     ----     
------------------------------------------


Welcome to Xinu!


xsh $ 
```

# Controlling the program

You now have complete control over the execution of your Xinu instance. In the gdb window you have access to the full power of GDB.

# Changing your code

If you make a change to your code, you do not need to exit gdb. From
the gdb commnand prompt, you can type **make** to rebuild you
code. Then you would need to type **load**. First use **ctrl-z** to
background gdb. Make your edits. Bring gdb back by typing **fg** on
the command line. You can now make and load the new program from gdb. 

```bash
(gdb) make
Rebuilding the .defs file

Loading object files to produce xinu
Image Name:
Created:      Fri Nov  4 10:09:50 2016
Image Type:   ARM Linux Kernel Image (uncompressed)
Data Size:    57344 Bytes = 56.00 kB = 0.05 MB
Load Address: 0x00010000
Entry Point:  0x00010000
(gdb) 
```
And then load the file:

```bash
(gdb) load
`/nfs/nfs6/home/bhimebau/forge/SOIC-Operating-Systems/P536-F16/compile/xinu' has changed; re-reading symbols.
Loading section .text, size 0xd000 lma 0x10000
Loading section .data, size 0x1000 lma 0x1d000
Start address 0x10000, load size 57344
Transfer rate: 56000 KB/sec, 1911 bytes/write.
```
You are now ready run using continue. 

# Automating GDB Startup

You may get tired of typing the setup commands into gdb every time
that you start gdb. This can be automated by place a file named
**.gdbinit** into your compile directory (or whereever you are going
to start gdb). This file can contain any valid gdb command. Here is an example file for my setup. This script causes gdb to connect to 55600 on silo, load symbols locally into gdb from the file xinu, sets a breakpoint at main, and then continues to that breakpoint. 

```bash
target extended-remote silo:5560
file xinu
b main
c
```





