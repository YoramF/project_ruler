libbig_numbers: big_numbers.o 
	ar rcs libbig_numbers.a big_numbers.o	

big_numbers.o: big_numbers.c big_numbers.h
	gcc -c big_numbers.c -o big_numbers.o

libbig_numbers_d: big_numbers_d.o
	ar rcs libbig_numbers_d.a big_numbers_d.o	

big_numbers_d.o: big_numbers.c big_numbers.h
	gcc -g -c big_numbers.c -o big_numbers_d.o

