CC=clang
CFLAGS=-std=c11 -Wall
EXTRACFLAGS=-g -O0
RANLIB=ranlib

OBJ=locklessqueue.o
DEPS=locklessqueue.h

all: $(OBJ) 
 
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(EXTRACFLAGS)
	
clean:
	rm *.o
