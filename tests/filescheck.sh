#!/bin/bash

counter=0

files="$(find $HOME/Entwicklung/c -type f -name '*.[ch]')"
for file in $files; do
	counter=$(("$counter" + 1))
	./lexer "$file" 1>/dev/null

done
echo "FILE COUNT: $counter"
