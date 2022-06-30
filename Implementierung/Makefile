# Add additional compiler flags here 
CFLAGS=-O0 -std=c11 -g -Wall -Wextra -Wpedantic

.PHONY: all 
all: main 
main:  salsa_crypt/salsa_crypt_v0.c main.c
	$(CC) $(CFLAGS) -o $@ $^ 
.PHONY: clean 
clean: 
	rm -f main