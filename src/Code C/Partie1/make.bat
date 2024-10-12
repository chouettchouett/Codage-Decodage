gcc -pedantic -Wextra -Wall sym_crypt_func.c -c
gcc -pedantic -Wextra -Wall sym_crypt.c sym_crypt_func.o -o sym_crypt.exe