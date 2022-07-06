# Add additional compiler flags here 
CFLAGS=-O0 -std=c11 -g -Wall -Wextra -Wpedantic

.PHONY: all 
all: main 
main:  file_IO/file_IO.c number_conversions/number_conversions.c salsa_crypt/salsa_crypt_v0.c salsa_crypt/salsa_crypt_v1.c salsa_crypt/salsa_crypt_v2.c salsa_core/rotate_left.c salsa_core/salsa_core_v0.c salsa_core/salsa_core_v1.c salsa_core/salsa_core_v2.c main.c
	$(CC) $(CFLAGS) -o $@ $^ 
.PHONY: clean 
clean: 
	rm -f main