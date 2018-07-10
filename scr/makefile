all: comp

comp:			main.o scan.o parser.o staticSemantics.o codeGeneration.o
			gcc -g -o comp main.o scan.o  parser.o staticSemantics.o codeGeneration.o

main.o:			main.c  parser.h codeGeneration.h 
			gcc -g -c main.c

scan.o:			scan.c token.h
			gcc -g -c scan.c

parser.o:		parser.c token.h scan.h  staticSemantics.h
			gcc -g -c parser.c

staticSemantics.o:	staticSemantics.c staticSemantics.h token.h
			gcc -g -c staticSemantics.c

codeGeneration.o:	codeGeneration.c codeGeneration.h parser.h token.h
			gcc -g -c codeGeneration.c
.PHONY:	clean

clean:
	rm *.o comp tmp.txt
	
veryclean:
	rm *.o comp *.asm tmp.txt
