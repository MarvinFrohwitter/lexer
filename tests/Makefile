# The lexer that tokenise the given C code.

SRC = dev-tests.c

CC = cc
LIBS = ../src/lexer.c
CFLAGS = -O3 -pedantic -Wall -Wextra -std=c11 -ggdb

all: options lexer

options:
	@echo lexer build options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "LIBS   = ${LIBS}"
	@echo "CC     = ${CC}"

lexer:
	${CC} -o $@ ${SRC} ${CFLAGS} ${LIBS}

clean:
	rm -f lexer

test: lexer
	./lexer


.PHONY: all clean options lexer test
