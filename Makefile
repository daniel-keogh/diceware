CC=gcc

diceware: diceware.c
	$(CC) -o diceware.o diceware.c -std=c11

clean: 
	rm diceware.o