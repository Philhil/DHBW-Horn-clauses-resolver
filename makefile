LEX  = flex
YACC = bison


all: scan_read
	
clean:
	rm scan_read
	rm *.o 
	rm lex.yy.c 

lex.yy.c: read.l
	lex read.l

grammar.tab.h: grammar.y
	$(YACC) -d grammar.y

grammar.tab.c: grammar.y
	$(YACC) -d grammar.y

grammar.tab.o: grammar.tab.c
	gcc -c -o grammar.tab.o grammar.tab.c

data.o: data.c
	gcc -c -o data.o data.c

lex.yy.o: lex.yy.c
	gcc -c -o lex.yy.o lex.yy.c

scan_read: lex.yy.o data.o grammar.tab.o 
	gcc lex.yy.o data.o grammar.tab.o -o scan_read


#bison -d grammar.y (generate tabc und tab.h)
#flex -t read.l > read.c
# gcc -c -o read.o read.c
# gcc -c -o grammar.tab.o grammar.tab.c
# gcc read.o grammar.tab.o -o read (linken)