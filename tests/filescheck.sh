#!/bin/bash

files="$(find /home/marvin/Entwicklung/c -type f -name '*.c')"
for file in $files
do
    ./lexer "$file" 1> /dev/null

done
