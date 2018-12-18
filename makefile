# Makefile do terceiro projeto de AED.
clean:
	rm -f finding matrixA.txt matrixB.txt

run:
	./finding 
all:
	cc -Wall -O2 finding.c -o finding
