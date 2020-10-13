CC = gcc
CFLAGS = -Wall -g -c
LDFLAGS =
OBJFILES = main.o
TARGET = a

all:$(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -Wall -g $(OBJFILES) -o $(TARGET) 

main.o: main.c
	$(CC) $(CFLAGS) main.c

