euler12: euler12.o divisors.o
	gcc -s -o euler12.exe euler12.o divisors.o

euler12.o: euler12.c divisors.h
	gcc -c -O3 euler12.c

divisors.o: divisors.c divisors.h
	gcc -c -O3 divisors.c

euler12_d: euler12_d.o divisors_d.o
	gcc -o euler12_d.exe euler12_d.o divisors_d.o

euler12_d.o: euler12.c divisors.h
	gcc -c -g euler12.c -o euler12_d.o

divisors_d.o: divisors.c divisors.h
	gcc -c -g divisors.c -o divisors_d.o

clean:
	rm *.o