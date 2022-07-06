CFLAGS=-O0 -std=gnu11 -g -Wall -Wextra -Wpedantic

.PHONY: all 
all: main 
main: ../../salsa_core/rotate_left.c rotateLeft_test.c 
	$(CC) $(CFLAGS) -o $@ $^ 
.PHONY: clean 
clean: 
	rm -f main