euler202:	euler202.o ../../timer/libtimer.a
	gcc -s euler202.o -o euler202.exe -ltimer -L../../timer

euler202.o:	euler202.c ../../timer/timer.h
	gcc -O3 -c euler202.c -o euler202.o

euler202_d:	euler202_d.o
	gcc euler202_d.o -o euler202_debug.exe -ltimer -L../../timer

euler202_d.o:	euler202.c
	gcc -g -c euler202.c -o euler202_d.o

clean:
	rm euler202*.o euler202*.exe