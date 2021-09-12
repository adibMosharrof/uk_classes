

all: Load Change Clean Print Query

Load: load.c header.h
	gcc -g -Wall -o Load load.c header.c
Change: change.c header.h
	gcc -g -Wall -o Change change.c header.c
Clean: clean.c header.h
	gcc -g -Wall -o Clean clean.c header.c
Print: print.c header.h
	gcc -g -Wall -o Print print.c header.c
Query: query.c header.h
	gcc -g -Wall -o Query query.c header.c
clean:
	rm -f Create  Change Change Print Query *~ core
