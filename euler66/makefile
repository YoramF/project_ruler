euler66_d: euler66.c ../big_numbers/libbig_numbers_d.a ../big_numbers/big_numbers.h
	gcc -g euler66.c -o euler66_debug.exe -lbig_numbers_d -L../big_numbers -I../big_numbers

euler66: euler66.c ../big_numbers/libbig_numbers.a ../big_numbers/big_numbers.h
	gcc -s euler66.c -o euler66.exe -lbig_numbers -L../big_numbers -I../big_numbers

euler66_gmp_d: euler66_gmp.c
	gcc -g euler66_gmp.c -o euler66_gmp_debug.exe -lgmp -lm

euler66_gmp: euler66_gmp.c
	gcc -s euler66_gmp.c -o euler66_gmp.exe -lgmp -lm
