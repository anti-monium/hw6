# Makefile for project 6hw
CC1=gcc
CC2=nasm
F1=-f elf32
F2=-c -m32
F3=-m32
all: 6.o 6hw.o
	$(CC1) 6.o 6hw.o $(F3)
6.o: 6.asm
	$(CC2) $(F1) 6.asm
6hw.o: 6hw.c
	$(CC1) 6hw.c $(F2)
clean:
	rm *.o
