CFLAGS=-O0 -std=gnu11 -g -Wall -Wextra -Wpedantic

.PHONY: all 
all: main 
main: ../../salsa_core/salsa_core_v1.c littleendian_test.c 
	$(CC) $(CFLAGS) -o $@ $^ 
.PHONY: clean 
clean: 
	rm -f main