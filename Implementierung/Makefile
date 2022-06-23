# Add additional compiler flags here 
CFLAGS=-O0 -std=c11 -g -Wall -Wextra -Wpedantic

.PHONY: all 
all: main 
main: main.c salsa20.h salsa20.c 
	$(CC) $(CFLAGS) -o $@ $^ 
.PHONY: clean 
clean: 
	rm -f main