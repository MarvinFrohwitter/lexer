CC = cc
LIBS = exlex.h
CFLAGS = -x c -O3 -pedantic -Wall -Wextra -std=c11 -ggdb

all: options lexer lexer-exlex

options:
	@echo Lexer build options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "LIBS   = ${LIBS}"
	@echo "CC     = ${CC}"


lexer:
	${CC} ${CFLAGS} -c lexer.c


lexer-exlex: lexer.h-exlex.h.o
	${CC} ${CFLAGS} -o lexer-exlex.o -c lexer.c

lexer.h-exlex.h.o: exlex.h.o
	${CC} ${CFLAGS} -o $@ -c lexer.h

exlex.h.o:
	${CC} -DEXLEX_IMPLEMENTATION ${CFLAGS} -o $@ -c exlex.h


clean:
	rm -f exlex.h.o lexer.h-exlex.h.o lexer-exlex.o lexer.o

.PHONY: all clean options lexer lexer-exlex lexer.h-exlex.h.o exlex.h.o
