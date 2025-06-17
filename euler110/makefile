euler110: euler110.o
	gcc -s -o euler110 euler110.o -lprime -L../prime -lm -lgmp

euler110.o: euler110.c 
	gcc -c euler110.c -o euler110.o -I../prime

euler110_d: euler110_d.o
	gcc -o euler110_d euler110_d.o -lprime_d -L../prime -lm -lgmp -ltimer -L../../timer

euler110_d.o: euler110.c ../../timer/timer.h
	gcc -g -c euler110.c -o euler110_d.o -I../prime -I../../timer -DDEBUG

test_f_d: test_f_d.o
	gcc -o test_f_d test_f_d.o -lprime_d -L../prime -lm

test_f_d.o: test_f.c 
	gcc -g -c test_f.c -o test_f_d.o -I../prime

factorization_d: factorization_d.o
	gcc -o factorization_d factorization_d.o

factorization_d.o: factorization.c 
	gcc -g -c factorization.c -o factorization_d.o