#!/bin/bash

res=".result"

nums="\
15 \
20 \
25 \
"

for num in ${nums}
	do\
		python3 checker.py -e ./a \
			-r result${num}.txt --max ${num} >> ${res}
	done

less ${res}
rm ${res}

