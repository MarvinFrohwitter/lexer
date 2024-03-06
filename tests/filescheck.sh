#!/bin/bash

counter=0

files="$(find /home/marvin/Entwicklung/c -type f -name '*.[ch]')"
for file in $files; do
	counter=$(("$counter" + 1))
	./lexer "$file" 1>/dev/null

done
echo $counter
