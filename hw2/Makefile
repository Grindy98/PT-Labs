
CC = gcc
CFLAGS = -Wall

all: demo.out
	@printf "\nThe output of demo.out is:\n"
	@./demo.out

demo.out: demo.o libmystring.a mystring.h
	$(CC) $(CFLAGS) -o demo.out demo.o -L. -lmystring

demo.o: demo.c
	$(CC) $(CFLAGS) -c -o demo.o demo.c 

libmystring.a: mystring.c
	$(CC) $(CFLAGS) -c -o mystring.o mystring.c
	ar -rcs libmystring.a mystring.o
	rm mystring.o

clean:
	rm *.o *.a demo.out 