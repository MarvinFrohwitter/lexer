# The lexer that tokenise the given C code.

SRC = dev-tests.c

CC = cc
LIBS = ../src/lexer.c
CFLAGS = -O0 -pedantic -Wall -Wextra -std=c11 -ggdb

all: options lexer

options:
	@echo lexer build options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "LIBS   = ${LIBS}"
	@echo "CC     = ${CC}"

lexer:
	${CC} -o lexer-intermediate ${SRC} ${CFLAGS} ${LIBS}
	${CC} -o $@ ${SRC} ${CFLAGS} -DEXLEX_IMPLEMENTATION ${LIBS}

clean:
	rm -f lexer lexer-intermediate

test: lexer
	./lexer-intermediate
	./lexer


.PHONY: all clean options lexer test
