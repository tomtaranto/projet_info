CFLAGS=-Wextra -Wall -ansi

all: prog

prog: structures.o csvIO.o affectation.o main.o
	gcc $^ -o $@

structures.o: structures.c structures.h
	gcc -c $(CFLAGS) $< 

affectation.o: affectation.c affectation.h structures.h
	gcc -c $(CFLAGS) $< 

csvIO.o: csvIO.c csvIO.h structures.h
	gcc -c $(CFLAGS) $<

main.o : main.c csvIO.h structures.h
	gcc -c $(CFLAGS) $<
