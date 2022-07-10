# Add additional compiler flags here 
<<<<<<< HEAD
CFLAGS=-O2 -std=c11 -g -Wall -Wextra -Wpedantic -fsanitize=address -fsanitize=undefined -fsanitize=leak -static-libasan -msse4.1
=======
CFLAGS=-Ofast -std=c11 -g -Wall -Wextra -Wpedantic -fsanitize=address -fsanitize=undefined -fsanitize=leak -static-libasan
>>>>>>> .

.PHONY: all 
all: main 
main:  file_IO/file_IO.c number_conversions/number_conversions.c salsa_crypt/salsa_crypt_v0.c salsa_crypt/salsa_crypt_v1.c salsa_crypt/salsa_crypt_v2.c salsa_core/rotate_left.c salsa_core/salsa_core_v0.c salsa_core/salsa_core_v1.c salsa_core/salsa_core_v2.c main.c
	$(CC) $(CFLAGS) -o $@ $^ 
.PHONY: clean 
clean: 
	rm -f main