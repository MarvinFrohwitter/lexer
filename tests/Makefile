# The lexer that tokenise the given C code.

SRC = dev-tests.c

CC = clang
INCLUDE = -I "${HOME}/Entwicklung/c/lexer/src/" -I "${HOME}/Entwicklung/c/gutils/"
LIBARY = -L "${HOME}/Entwicklung/c/lexer/src/"
LIBS = ../src/lexer.c
CFLAGS = -O3 -pedantic -Wall -Wextra -std=c11 -ggdb
CFLAGS += -static
DEFINES = -DLEX_LOGERROR -DEXLEX_IMPLEMENTATION

all: options lexer

options:
	@echo "------------------------------------------------------------------"
	@echo "Embedded lexer build options:"
	@echo "CFLAGS = ${CFLAGS}"
	@echo "LIBS   = ${LIBS}"
	@echo "CC     = ${CC}"
	@echo "------------------------------------------------------------------"

lexer:
	$(MAKE) -C ${HOME}/Entwicklung/c/lexer/src/
	${CC} ${DEFINES} ${CFLAGS} ${INCLUDE} -o lexer-intermediate ${SRC} ${LIBARY} -llexer
	${CC} ${DEFINES} ${CFLAGS} ${INCLUDE} -o $@ ${SRC} ${LIBS}

clean:
	rm -f lexer lexer-intermediate

test: lexer
	# ./lexer-intermediate
	./lexer


.PHONY: all clean options lexer test
