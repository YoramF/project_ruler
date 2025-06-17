euler201:	euler201.o
	gcc -s euler201.o -o euler201.exe

euler201.o:	euler201.c
	gcc -O3 -c euler201.c -o euler201.o

euler201_d:	euler201_d.o
	gcc euler201_d.o -o euler201_debug.exe

euler201_d.o:	euler201.c
	gcc -g -c euler201.c -o euler201_d.o

clean:
	rm euler201*.o euler201*.exe