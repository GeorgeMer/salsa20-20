# Add additional compiler flags here 
CC=gcc
CFLAGS=-O2 -std=c11 -msse4.1
CFLAGS_DEBUG = -Wall -Wextra -Wpedantic -fsanitize=address -fsanitize=undefined -fsanitize=leak -static-libasan -Wshadow -Wdouble-promotion -Wformat=2 -Wformat-truncation -Wundef -fno-common -Wconversion -Wmisleading-indentation -g3

all: main 
debug:  file_IO/file_IO.c testing/reference/correctness.c testing/reference/reference.c testing/asserts.c testing/print_tests.c testing/random_generation/random_msg_key_iv.c testing/random_generation/random_numbers.c number_conversions/number_conversions.c salsa_crypt/salsa_crypt_v0.c salsa_crypt/salsa_crypt_v1.c salsa_crypt/salsa_crypt_v2.c salsa_core/rotate_left.c salsa_core/salsa_core_v0.c salsa_core/salsa_core_v1.c salsa_core/salsa_core_v2.c main.c
	$(CC) -o salsa $^ $(CFLAGS) $(CFLAGS_DEBUG)
main:  file_IO/file_IO.c testing/reference/correctness.c testing/reference/reference.c testing/asserts.c testing/print_tests.c testing/random_generation/random_msg_key_iv.c testing/random_generation/random_numbers.c number_conversions/number_conversions.c salsa_crypt/salsa_crypt_v0.c salsa_crypt/salsa_crypt_v1.c salsa_crypt/salsa_crypt_v2.c salsa_core/rotate_left.c salsa_core/salsa_core_v0.c salsa_core/salsa_core_v1.c salsa_core/salsa_core_v2.c main.c
	$(CC) -o salsa $^ $(CFLAGS)
clean: 
	rm -f main