output: sys_info.o
	gcc sys_info.o -o output

sys_info.o: sys_info.c
	gcc -c sys_info.c

clean:
	rm *.o output 

run: output
	./output date
