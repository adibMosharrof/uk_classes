

all: load change del print query

load: load.c header.h
	gcc -g -Wall -o load load.c header.c
change: change.c header.h
	gcc -g -Wall -o change change.c header.c
del: clean.c header.h
	gcc -g -Wall -o del clean.c header.c
print: print.c header.h
	gcc -g -Wall -o print print.c header.c
query: query.c header.h
	gcc -g -Wall -o query query.c header.c
clean:
	rm -f create  change del print query *~ core
