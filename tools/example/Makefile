SRCS=example.c rv.c
HDRS=rv.h
CC=gcc
INCLUDES=-I../ch32v003fun/fake
CFLAGS=$(INCLUDES) -Wall -Wextra -pedantic \
	-Wshadow -Wswitch -Wfloat-equal -pedantic \
	-g -Os -flto
# DO NOT USE -Os -flto -fstack-protector-all combination on MINGW!!!!!!!
# Will cause weird overflow error on CPU.PC
# note: it is your program bug, not the compiler bug

example: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(SRCS) -o $@

clean:
	rm -rf example
	$(MAKE) -f Makefile1 clean

riscv:
	$(MAKE) -f Makefile1 clean
	$(MAKE) -f Makefile1
	$(MAKE) -f Makefile1 size

.PHONY: clean riscv
