ebedecl: parse.o lex.o main.o tokenmap.o symbols.o
	g++ -g -o ebedecl parse.o lex.o main.o tokenmap.o symbols.o -ll

parse.o: parse.c
	g++ -g -c parse.c

symbols.o: symbols.cpp parse.h
	g++ -g -c symbols.cpp

lex.o: lex.c
	g++ -g -c lex.c

main.o: main.cpp
	g++ -g -c main.cpp

tokenmap.o: tokenmap.cpp parse.h
	g++ -g -c tokenmap.cpp

lex.c: lex.l parse.h
	flex -o lex.c lex.l

parse.c: parse.y
	bison --defines=parse.h -o parse.c parse.y

parse.h: parse.y
	bison --defines=parse.h -o parse.c parse.y

install: ebedecl
	cp ebedecl /usr/bin

clean:
	rm -f ebedecl *.o lex.h parse.h lex.c parse.c
