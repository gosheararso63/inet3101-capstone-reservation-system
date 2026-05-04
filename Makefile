CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = reservation_system
OBJS = main.o reservation.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lgdbm

main.o: main.c reservation.h
	$(CC) $(CFLAGS) -c main.c

reservation.o: reservation.c reservation.h
	$(CC) $(CFLAGS) -c reservation.c

clean:
	rm -f $(TARGET) $(OBJS) reservations.gdbm
