libprime: prime.o
	ar rcs libprime.a prime.o

libprime_gmp: prime_gmp.o
	ar rcs libprime_gmp.a prime_gmp.o

prime.o: prime.c prime.h
	gcc -c prime.c -o prime.o

prime_gmp.o: prime_gmp.c
	gcc -c prime_gmp.c -o prime_gmp.o

libprime_d: prime_d.o
	ar rcs libprime_d.a prime_d.o

libprime_gmp_d: prime_gmp_d.o
	ar rcs libprime_gmp_d.a prime_gmp_d.o

prime_d.o: prime.c prime.h
	gcc -g -c prime.c -o prime_d.o

prime_gmp_d.o: prime_gmp.c
	gcc -g -c prime_gmp.c -o prime_gmp_d.o

tst_d: tst.c libprime_d.a
	gcc -g tst.c -o tst_d -lprime_d -lprime_gmp_d-L . -lm -lgmp
 
tst: tst.c libprime.a
	gcc -s tst.c -o tst -lprime -lprime_gmp -L . -lm -lgmp