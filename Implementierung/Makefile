# Add additional compiler flags here 
CFLAGS=-O2 -std=c11 -g -Wall -Wextra -Wpedantic -fsanitize=address -fsanitize=undefined -fsanitize=leak -static-libasan -msse4.1

.PHONY: all 
all: main 
main:  file_IO/file_IO.c testing/reference/correctness.c testing/reference/reference.c testing/asserts.c testing/print_tests.c testing/random_generation/random_msg_key_iv.c testing/random_generation/random_numbers.c number_conversions/number_conversions.c salsa_crypt/salsa_crypt_v0.c salsa_crypt/salsa_crypt_v1.c salsa_crypt/salsa_crypt_v2.c salsa_core/rotate_left.c salsa_core/salsa_core_v0.c salsa_core/salsa_core_v1.c salsa_core/salsa_core_v2.c main.c
	$(CC) $(CFLAGS) -o $@ $^ 
.PHONY: clean 
clean: 
	rm -f main