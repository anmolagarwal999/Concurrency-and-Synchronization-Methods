CC = gcc
CFLAGS = -Wall -g -c
LDFLAGS =
OBJFILES = main.o companies.o students.o hospitals.o utils.o
TARGET = a

all:$(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -Wall -g $(OBJFILES) -lpthread -o $(TARGET) 

main.o: main.c
	$(CC) $(CFLAGS) main.c

companies.o: companies.c
	$(CC) $(CFLAGS) companies.c

students.o: students.c
	$(CC) $(CFLAGS) students.c

hospitals.o: hospitals.c
	$(CC) $(CFLAGS) hospitals.c


utils.o: utils.c
	$(CC) $(CFLAGS) utils.c


