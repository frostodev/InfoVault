CC = gcc
CFLAGS = -Wall -Wextra -g
OBJS = Manufactura.o Robot.o Mundo.o Io.o FactoryOfNumbers.o

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o FactoryOfNumbers

Manufactura.o: Manufactura.c
	$(CC) $(CFLAGS) -c Manufactura.c -o Manufactura.o

Mundo.o: Mundo.c
	$(CC) $(CFLAGS) -c Mundo.c -o Mundo.o

Robot.o: Robot.c
	$(CC) $(CFLAGS) -c Robot.c -o Robot.o

Io.o: Io.c
	$(CC) $(CFLAGS) -c Io.c -o Io.o

FactoryOfNumbers.o: FactoryOfNumbers.c
	$(CC) $(CFLAGS) -c FactoryOfNumbers.c -o FactoryOfNumbers.o

clean:
	rm -f $(OBJS) main