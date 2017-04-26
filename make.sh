#!/bin/sh
bison -d grammar.y
flex -t read.l > read.c
gcc -c -o read.o read.c
gcc -c -o grammar.tab.o grammar.tab.c
gcc read.o grammar.tab.o -o read
