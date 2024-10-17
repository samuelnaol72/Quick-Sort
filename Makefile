CC = gcc
CFLAGS = -W -Wall

all: test minfine

test: quick.o test.o 
	$(CC) $(CFLAGS) -o test quick.o test.o

minfine: minfine.o quick.o
	$(CC) $(CFLAGS) -o minfine minfine.o quick.o 

%.o: %.c
	$(CC) $(CFLAGS) $< -c

quick.o: quick.c quick.h

test.o: test.c

clean:
	@rm *.o test minfine