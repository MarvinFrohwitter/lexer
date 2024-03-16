CC = clang
CFLAGS = -x c -O3 -pedantic -Wall -Wextra -std=c11 -ggdb -pg
DEFINES = -DLEX_LOGERROR -DEXLEX_IMPLEMENTATION

all: options lexer

options:
	@echo "------------------------------------------------------------------"
	@echo "Lexer build options:"
	@echo "CFLAGS = ${CFLAGS}"
	@echo "LIBS   = ${LIBS}"
	@echo "CC     = ${CC}"
	@echo "------------------------------------------------------------------"

lexer:
	${CC} ${DEFINES} ${CFLAGS} -c lexer.c
	ar -r liblexer.a lexer.o

clean:
	rm -f lexer.o liblexer.a

.PHONY: all clean options lexer
