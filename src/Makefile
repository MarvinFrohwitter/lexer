CC = clang
LIBS =
CFLAGS = -x c -O3 -pedantic -Wall -Wextra -std=c11 -ggdb

all: options lexer

options:
	@echo Lexer build options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "LIBS   = ${LIBS}"
	@echo "CC     = ${CC}"

lexer:
	${CC} ${CFLAGS} -c lexer.c

clean:
	rm -f lexer.o

.PHONY: all clean options lexer
